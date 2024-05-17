void GDataCache::GetResourceIdsOfExistingPinnedFiles(
    std::vector<std::string>* resource_ids) {
  AssertOnSequencedWorkerPool();
  DCHECK(resource_ids);

  metadata_->Iterate(base::Bind(&CollectExistingPinnedFile, resource_ids));
}
