bool HFSIterator::IsDecmpfsCompressed() {
  if (IsDirectory())
    return false;
  const HFSPlusCatalogFile* file = catalog_->current_record()->file;
  return file->bsdInfo.ownerFlags & UF_COMPRESSED;
}
