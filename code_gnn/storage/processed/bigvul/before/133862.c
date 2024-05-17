bool HFSIterator::IsDirectory() {
  return catalog_->current_record()->record_type == kHFSPlusFolderRecord;
}
