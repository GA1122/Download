bool HFSIterator::IsHardLink() {
  if (IsDirectory())
    return false;
  const HFSPlusCatalogFile* file = catalog_->current_record()->file;
  return file->userInfo.fdType == kHardLinkFileType &&
         file->userInfo.fdCreator == kHFSPlusCreator;
}
