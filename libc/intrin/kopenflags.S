/*-*- mode:unix-assembly; indent-tabs-mode:t; tab-width:8; coding:utf-8     -*-│
│vi: set et ft=asm ts=8 tw=8 fenc=utf-8                                     :vi│
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
#include "libc/fmt/magnumstrs.internal.h"
#include "libc/macros.internal.h"

	.macro	.e e s
	.long	\e - kOpenFlags
	.long	.L\@ - kOpenFlags
	.rodata.str1.1
.L\@:	.string	"\s"
	.previous
	.endm

	.section .rodata
	.align	4
	.underrun
kOpenFlags:
	.e	O_RDWR,"RDWR"			// order matters
	.e	O_RDONLY,"RDONLY"		//
	.e	O_WRONLY,"WRONLY"		//
	.e	O_ACCMODE,"ACCMODE"		// mask of prev three
	.e	O_CREAT,"CREAT"			//
	.e	O_EXCL,"EXCL"			//
	.e	O_TRUNC,"TRUNC"			//
	.e	O_CLOEXEC,"CLOEXEC"		//
	.e	O_NONBLOCK,"NONBLOCK"		//
	.e	O_TMPFILE,"TMPFILE"		// linux, windows
	.e	O_DIRECTORY,"DIRECTORY"		// order matters
	.e	O_DIRECT,"DIRECT"		// no-op on xnu/openbsd
	.e	O_APPEND,"APPEND"		// weird on nt
	.e	O_NOFOLLOW,"NOFOLLOW"		// unix
	.e	O_SYNC,"SYNC"			// unix
	.e	O_ASYNC,"ASYNC"			// unix
	.e	O_NOCTTY,"NOCTTY"		// unix
	.e	O_NOATIME,"NOATIME"		// linux
	.e	O_EXEC,"EXEC"			// free/openbsd
	.e	O_SEARCH,"SEARCH"		// free/netbsd
	.e	O_DSYNC,"DSYNC"			// linux/xnu/open/netbsd
	.e	O_RSYNC,"RSYNC"			// linux/open/netbsd
	.e	O_PATH,"PATH"			// linux
	.e	O_VERIFY,"VERIFY"		// freebsd
	.e	O_SHLOCK,"SHLOCK"		// bsd
	.e	O_EXLOCK,"EXLOCK"		// bsd
	.e	O_RANDOM,"RANDOM"		// windows
	.e	O_SEQUENTIAL,"SEQUENTIAL"	// windows
	.e	O_COMPRESSED,"COMPRESSED"	// windows
	.e	O_INDEXED,"INDEXED"		// windows
	.e	O_LARGEFILE,"LARGEFILE"		//
	.long	MAGNUM_TERMINATOR
	.endobj	kOpenFlags,globl,hidden
	.overrun
