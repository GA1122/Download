void SBEntry::SetChunkIdAtPrefix(int index, int chunk_id) {
  DCHECK(IsSub());

  if (type() == SUB_PREFIX)
    sub_prefixes_[index].add_chunk = chunk_id;
  else
    sub_full_hashes_[index].add_chunk = chunk_id;
}
