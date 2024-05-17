int SBEntry::ChunkIdAtPrefix(int index) const {
  if (type() == SUB_PREFIX)
    return sub_prefixes_[index].add_chunk;
  return (type() == SUB_FULL_HASH) ?
      sub_full_hashes_[index].add_chunk : chunk_id();
}
