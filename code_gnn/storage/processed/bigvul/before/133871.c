bool HFSBTreeIterator::ReadCurrentLeaf() {
  if (read_current_leaf_)
    return true;

  if (!SeekToNode(current_leaf_number_)) {
    DLOG(ERROR) << "Failed to seek to node " << current_leaf_number_;
    return false;
  }

  if (!stream_->ReadExact(&leaf_data_[0], header_.nodeSize)) {
    DLOG(ERROR) << "Failed to read node " << current_leaf_number_;
    return false;
  }

  auto leaf = reinterpret_cast<BTNodeDescriptor*>(&leaf_data_[0]);
  ConvertBigEndian(leaf);
  if (leaf->kind != kBTLeafNode) {
    DLOG(ERROR) << "Node " << current_leaf_number_ << " is not a leaf";
    return false;
  }
  current_leaf_ = leaf;
  current_leaf_offset_ = sizeof(BTNodeDescriptor);
  current_leaf_records_read_ = 0;
  read_current_leaf_ = true;
  return true;
}
