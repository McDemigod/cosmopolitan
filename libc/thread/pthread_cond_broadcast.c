/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
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
#include "libc/thread/thread.h"
#include "third_party/nsync/cv.h"

/**
 * Wakes all threads waiting on condition, e.g.
 *
 *     pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 *     pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 *     // ...
 *     pthread_mutex_lock(&lock);
 *     pthread_cond_broadcast(&cond, &lock);
 *     pthread_mutex_unlock(&lock);
 *
 * This function has no effect if there aren't any threads currently
 * waiting on the condition.
 *
 * @return 0 on success, or errno on error
 * @see pthread_cond_signal
 * @see pthread_cond_wait
 */
errno_t pthread_cond_broadcast(pthread_cond_t *cond) {
  nsync_cv_broadcast((nsync_cv *)cond);
  return 0;
}
