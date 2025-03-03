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
#include "libc/calls/syscall-nt.internal.h"
#include "libc/calls/syscall-sysv.internal.h"
#include "libc/dce.h"
#include "libc/intrin/strace.internal.h"
#include "libc/macros.internal.h"
#include "libc/sysv/consts/msync.h"

/**
 * Synchronize memory mapping changes to disk.
 *
 * Without this, there's no guarantee memory is written back to disk.
 * Particularly on RHEL5, OpenBSD, and Windows NT.
 *
 * @param addr needs to be 4096-byte page aligned
 * @param flags needs MS_ASYNC or MS_SYNC and can have MS_INVALIDATE
 * @return 0 on success or -1 w/ errno
 */
int msync(void *addr, size_t size, int flags) {
  int rc;
  _unassert(((flags & MS_SYNC) ^ (flags & MS_ASYNC)) || !(MS_SYNC && MS_ASYNC));
  if (!IsWindows()) {
    rc = sys_msync(addr, size, flags);
  } else {
    rc = sys_msync_nt(addr, size, flags);
  }
  STRACE("msync(%p, %'zu, %#x) → %d% m", addr, size, flags, rc);
  return rc;
}
