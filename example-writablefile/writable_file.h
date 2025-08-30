#pragma once

#include <algorithm>
#include <cassert>
#include <errno.h>
#include <unistd.h>

#include "slice.h"
#include "status.h"

constexpr const size_t kWritableFileBufferSize = 65536;

Status PosixError(const char* filename, int errcode);

class WritableFile {
  public:
    WritableFile() = default;

    virtual ~WritableFile() = default;

    WritableFile(const WritableFile&) = delete;
    WritableFile& operator=(const WritableFile&) = delete;

    virtual Status Append(const Slice& data) = 0;
    virtual Status Close() = 0;
    virtual Status Flush() = 0;
    virtual Status Sync() = 0;
};

class WritableFileImpl : public WritableFile {
  public:
    WritableFileImpl(const std::string& filename, int fd)
    : pos_(0),
      fd_(fd),
      filename_(filename),
      dirname_(Dirname(filename)) {
    }

    ~WritableFileImpl() override {
      if (fd_ >= 0) {
        Close();
      }
    }

    Status Append(const Slice& data) override {
      size_t write_size = data.size();
      const char* write_data = data.data();

      size_t copy_size = std::min(write_size, kWritableFileBufferSize - pos_);
      std::memcpy(buf_ + pos_, write_data, copy_size);
      write_data += copy_size;
      write_size -= copy_size;
      pos_ += copy_size;
      if (write_size == 0) {
        return Status::OK();
      }
      Status r = FlushBuffer();
      if (!r.ok()) {
        return r;
      }

      if (write_size < kWritableFileBufferSize) {
        std::memcpy(buf_, write_data, write_size);
        pos_ = write_size;
        return Status::OK();
      }

      return WriteUnbuffered(write_data, write_size);
    }

    Status Close() override {
      Status s = FlushBuffer();
      const int close_result = ::close(fd_);
      if (close_result < 0 && s.ok()) {
        s = PosixError(filename_.c_str(), errno);
      }
      fd_ = -1;
      return s;
    }

    Status Flush() override {
      Status s = FlushBuffer();
      if (!s.ok()) {
        return s;
      }
      return SyncFd(fd_, filename_);
    }

    Status Sync() override {
      return Status::NotSupported("");
    }

  private:
    static std::string Dirname(const std::string& filename) {
      std::string::size_type seperator_pos = filename.rfind('/');
      if (seperator_pos == std::string::npos) {
        return std::string(".");
      }
      assert(filename.find('/', seperator_pos + 1) == std::string::npos);

      return filename.substr(0, seperator_pos);
    }

    Status FlushBuffer() {
      Status r = WriteUnbuffered(buf_, pos_);
      // FIXME(yutao): write partial
      pos_ = 0;
      return r;
    }

    Status WriteUnbuffered(const char* data, size_t size) {
      while (size > 0) {
        ssize_t write_result = ::write(fd_, data, size);
        if (write_result < 0) {
          if (errno == EINTR) {
            continue;
          }
          return PosixError(filename_.c_str(), errno);
        }
        data += write_result;
        size -= write_result;
      }
      return Status::OK();
    }

    static Status SyncFd(int fd, const std::string& filename) {
      bool sync_success = ::fsync(fd);
      if (sync_success) {
        return Status::OK();
      }
      return PosixError(filename.c_str(), errno);
    }


    char buf_[kWritableFileBufferSize];
    size_t pos_;
    int fd_;

    const std::string filename_;
    const std::string dirname_;
};

Status RemoveFile(const std::string& fname);

Status NewWritableFile(const std::string& fname, WritableFile** file);

Status DoWriteStringToFile(const Slice& data, const std::string& fname, bool should_sync);
