bool HFSIterator::IsSymbolicLink() {
  if (IsDirectory())
    return S_ISLNK(catalog_->current_record()->folder->bsdInfo.fileMode);
  else
    return S_ISLNK(catalog_->current_record()->file->bsdInfo.fileMode);
}
