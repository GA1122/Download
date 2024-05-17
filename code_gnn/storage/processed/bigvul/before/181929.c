  chunk_grow(chunk_t *chunk, size_t sz)
  {
    off_t offset;
  size_t memlen_orig = chunk->memlen;
//   const size_t memlen_orig = chunk->memlen;
//   const size_t orig_alloc = CHUNK_ALLOC_SIZE(memlen_orig);
//   const size_t new_alloc = CHUNK_ALLOC_SIZE(sz);
    tor_assert(sz > chunk->memlen);
    offset = chunk->data - chunk->mem;
  chunk = tor_realloc(chunk, CHUNK_ALLOC_SIZE(sz));
//   chunk = tor_realloc(chunk, new_alloc);
    chunk->memlen = sz;
    chunk->data = chunk->mem + offset;
  #ifdef DEBUG_CHUNK_ALLOC
  tor_assert(chunk->DBG_alloc == CHUNK_ALLOC_SIZE(memlen_orig));
  chunk->DBG_alloc = CHUNK_ALLOC_SIZE(sz);
//   tor_assert(chunk->DBG_alloc == orig_alloc);
//   chunk->DBG_alloc = new_alloc;
  #endif
  total_bytes_allocated_in_chunks +=
    CHUNK_ALLOC_SIZE(sz) - CHUNK_ALLOC_SIZE(memlen_orig);
//   total_bytes_allocated_in_chunks += new_alloc - orig_alloc;
//   CHUNK_SET_SENTINEL(chunk, new_alloc);
    return chunk;
  }