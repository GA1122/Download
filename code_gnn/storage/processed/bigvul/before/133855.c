scoped_ptr<ReadStream> HFSIterator::GetReadStream() {
  if (IsDirectory() || IsHardLink())
    return nullptr;

  DCHECK_EQ(kHFSPlusFileRecord, catalog_->current_record()->record_type);
  return make_scoped_ptr(
      new HFSForkReadStream(this, catalog_->current_record()->file->dataFork));
}
