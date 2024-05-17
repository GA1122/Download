bool HFSBTreeIterator::SeekToNode(uint32_t node_id) {
  if (node_id >= header_.totalNodes)
    return false;
  size_t offset = node_id * header_.nodeSize;
  if (stream_->Seek(offset, SEEK_SET) != -1) {
    current_leaf_number_ = node_id;
    return true;
  }
  return false;
}
