#ifndef BUF_H_
#define BUF_H_

#include <stddef.h>

/*
 * Implements a dynamically resizing buffer
 */
struct buf;

/*
 * Allocates a new dynamic buffer
 * Optionally specify initial size
 */
struct buf *buf_alloc(int sz);

/*
 * Creates a new dynamic buffer by taking ownership of an existing buffer
 * b - The buffer to own
 * sz - The buffer's size
 * Returns: The new dynamic buffer, with its initial contents set to the owned
 * buffer
 */
struct buf *buf_own(void *b, int sz);

/*
 * Close the buffer, freeing its resources
 */
void buf_free(struct buf *);

/*
 * Read a set number of bytes from the buffer
 * pos - The position to read from
 * cnt - The number of bytes to read
 * tgt - The buffer to read into
 * Returns: The number of bytes read
 */
size_t buf_read(struct buf const *, size_t pos, size_t cnt, char *tgt);

/*
 * Writes a set number of bytes to the buffer
 * This operation potentially reallocates the buffer
 * pos - The position to write to
 * cnt - The number of bytes to write
 * tgt - The buffer to write from
 */
void buf_write(struct buf **, size_t pos, size_t cnt, char const *tgt);

/*
 * Get the current size of the buffer
 */
size_t buf_sz(struct buf const *);

#endif // BUF_H_
