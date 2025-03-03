/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
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
#include "ape/sections.internal.h"
#include "libc/calls/internal.h"
#include "libc/calls/metalfile.internal.h"
#include "libc/intrin/weaken.h"
#include "libc/mem/mem.h"
#include "libc/runtime/runtime.h"
#include "libc/str/str.h"
#include "libc/sysv/errfuns.h"

int sys_openat_metal(int dirfd, const char *file, int flags, unsigned mode) {
  int fd;
  struct MetalFile *state;
  if (strcmp(file, "ape.com")) return enoent();
  if (!_weaken(calloc)) return enomem();
  if ((fd = __reservefd(-1)) == -1) return -1;
  state = _weaken(calloc)(1, sizeof(struct MetalFile));
  state->base = (char *)_base;
  state->size = _end - _base;
  g_fds.p[fd].kind = kFdFile;
  g_fds.p[fd].flags = flags;
  g_fds.p[fd].mode = mode;
  g_fds.p[fd].handle = (intptr_t)state;
  return fd;
}
