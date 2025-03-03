/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/assert.h"
#include "libc/calls/calls.h"
#include "libc/calls/internal.h"
#include "libc/intrin/strace.internal.h"
#include "libc/calls/struct/stat.h"
#include "libc/calls/syscall-sysv.internal.h"
#include "libc/dce.h"
#include "libc/errno.h"
#include "libc/fmt/fmt.h"
#include "libc/fmt/itoa.h"
#include "libc/log/log.h"
#include "libc/nt/console.h"
#include "libc/nt/enum/consolemodeflags.h"
#include "libc/str/str.h"
#include "libc/sysv/errfuns.h"

static textwindows dontinline int sys_ttyname_nt(int fd, char *buf,
                                                 size_t size) {
  uint32_t mode;
  if (GetConsoleMode(g_fds.p[fd].handle, &mode)) {
    if (mode & kNtEnableVirtualTerminalInput) {
      strncpy(buf, "CONIN$", size);
      return 0;
    } else {
      strncpy(buf, "CONOUT$", size);
      return 0;
    }
  } else {
    return enotty();
  }
}

static int ttyname_freebsd(int fd, char *buf, size_t size) {
  const unsigned FIODGNAME = 2148558456;
  struct fiodgname_arg {
    int len;
    void *buf;
  } fg;
  fg.buf = buf;
  fg.len = size;
  if (sys_ioctl(fd, FIODGNAME, &fg) != -1) return 0;
  return enotty();
}

static int ttyname_linux(int fd, char *buf, size_t size) {
  struct stat st1, st2;
  if (!isatty(fd)) return errno;
  char name[PATH_MAX];
  FormatInt32(stpcpy(name, "/proc/self/fd/"), fd);
  ssize_t got;
  got = readlink(name, buf, size);
  if (got == -1) return errno;
  if ((size_t)got >= size) return erange();
  buf[got] = 0;
  if (stat(buf, &st1) || fstat(fd, &st2)) return errno;
  if (st1.st_dev != st2.st_dev || st1.st_ino != st2.st_ino) return enodev();
  return 0;
}

/**
 * Returns name of terminal, reentrantly.
 */
int ttyname_r(int fd, char *buf, size_t size) {
  int rc;
  if (IsLinux()) {
    rc = ttyname_linux(fd, buf, size);
  } else if (IsFreebsd()) {
    rc = ttyname_freebsd(fd, buf, size);
  } else if (IsWindows()) {
    if (__isfdkind(fd, kFdFile)) {
      rc = sys_ttyname_nt(fd, buf, size);
    } else {
      rc = ebadf();
    }
  } else {
    rc = enosys();
  }
  STRACE("ttyname_r(%d, %s) → %d% m", fd, buf, rc);
  return rc;
}
