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
#include "libc/calls/calls.h"
#include "libc/macros.internal.h"
#include "libc/runtime/directmap.internal.h"
#include "libc/runtime/pc.internal.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/map.h"
#include "libc/sysv/consts/prot.h"
#include "libc/sysv/errfuns.h"

static uint64_t sys_mmap_metal_break;

noasan struct DirectMap sys_mmap_metal(void *paddr, size_t size, int prot,
                                       int flags, int fd, int64_t off) {
  /* asan runtime depends on this function */
  size_t i;
  struct mman *mm;
  struct DirectMap res;
  uint64_t addr, page, *pte, *pml4t;
  mm = (struct mman *)(BANE + 0x0500);
  pml4t = __get_pml4t();
  size = ROUNDUP(size, 4096);
  addr = (uint64_t)paddr;
  if (!(flags & MAP_FIXED)) {
    if (!addr)
      addr = 4096;
    for (i = 0; i < size; i += 4096) {
      pte = __get_virtual(mm, pml4t, addr + i, false);
      if (pte && (*pte & (PAGE_V | PAGE_RSRV))) {
        addr = MAX(addr, sys_mmap_metal_break) + i + 4096;
        i = 0;
      }
    }
    sys_mmap_metal_break = MAX(addr + size, sys_mmap_metal_break);
  }
  for (i = 0; i < size; i += 4096) {
    page = __new_page(mm);
    pte = __get_virtual(mm, pml4t, addr + i, true);
    if (pte && page) {
      __clear_page(BANE + page);
      page |= PAGE_RSRV | PAGE_U;
      if ((prot & PROT_WRITE))
        page |= PAGE_V | PAGE_RW;
      else if ((prot & (PROT_READ | PROT_EXEC)))
        page |= PAGE_V;
      if (!(prot & PROT_EXEC)) page |= PAGE_XD;
      *pte = page;
    } else {
      addr = -1;
      break;
    }
  }
  res.addr = (void *)addr;
  res.maphandle = -1;
  return res;
}
