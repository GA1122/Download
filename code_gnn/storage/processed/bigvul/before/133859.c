bool HFSBTreeIterator::HasNext() {
  return leaf_records_read_ < header_.leafRecords;
}
