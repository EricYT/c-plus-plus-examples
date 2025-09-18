#pragma once

#if !defined(THREAD_ANNOTATION_ATTRIBUTES__)

#if defined(__clang__)

#define THREAD_ANNOTATION_ATTRIBUTES__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTES__(x)  // no op

#endif

#endif

#ifndef LOCKABLE
#define LOCKABLE THREAD_ANNOTATION_ATTRIBUTES__(lockable)
#endif

#ifndef GUARDED_BY
#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTES__(guarded_by(x))
#endif

#ifndef SCOPED_LOCKABLE
#define SCOPED_LOCKABLE THREAD_ANNOTATION_ATTRIBUTES__(scoped_lockable)
#endif

#ifndef EXCLUSIVE_LOCK_FUNCTION
#define EXCLUSIVE_LOCK_FUNCTION(...) \
  THREAD_ANNOTATION_ATTRIBUTES__(exclusive_lock_function(__VA_ARGS__))
#endif

#ifndef UNLOCK_FUNCTION
#define UNLOCK_FUNCTION(...) \
  THREAD_ANNOTATION_ATTRIBUTES__(unlock_function(__VA_ARGS__))
#endif

#ifndef ASSERT_EXCLUSIVE_LOCK
#define ASSERT_EXCLUSIVE_LOCK(...) \
  THREAD_ANNOTATION_ATTRIBUTES__(assert_exclusive_lock(__VA_ARGS__))
#endif
