Status IndexedDBDatabase::OpenInternal() {
  bool found = false;
  Status s = metadata_coding_->ReadMetadataForDatabaseName(
      backing_store_->db(), backing_store_->origin_identifier(), metadata_.name,
      &metadata_, &found);
  DCHECK(found == (metadata_.id != kInvalidId))
      << "found = " << found << " id = " << metadata_.id;
  if (!s.ok() || found)
    return s;

  return metadata_coding_->CreateDatabase(
      backing_store_->db(), backing_store_->origin_identifier(), metadata_.name,
      metadata_.version, &metadata_);
}
