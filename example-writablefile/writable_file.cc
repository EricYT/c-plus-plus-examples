#include <fcntl.h>
#include <unistd.h>

#include "writable_file.h"

Status PosixError(const char* filename, int errcode) {
  if (errcode == ENOENT) {
    return Status::NotFound(filename, std::strerror(errcode));
  } else {
    return Status::IOError(filename, std::strerror(errcode));
  }
}

Status RemoveFile(const std::string& fname) {
  if (::unlink(fname.c_str()) != 0) {
    return PosixError(fname.c_str(), errno);
  }
  return Status::OK();
}

Status NewWritableFile(const std::string& fname, WritableFile** file) {
  int fd = ::open(fname.c_str(),
     O_TRUNC | O_WRONLY | O_CREAT, 0644);
  if (fd < 0) {
    *file = nullptr;
    return PosixError(fname.c_str(), errno);
  }
  *file = new WritableFileImpl(fname, fd);
  return Status::OK();
}

Status DoWriteStringToFile(const Slice& data, const std::string& fname, bool should_sync) {
  WritableFile* file;
  Status s = NewWritableFile(fname, &file);
  if (!s.ok()) {
    return s;
  }
  s = file->Append(data);
  if (s.ok() && should_sync) {
    s = file->Sync();
  }
  if (s.ok()) {
    s = file->Close();
  }
  delete file;
  if (!s.ok()) {
    RemoveFile(fname);
  }
  return s;
}
