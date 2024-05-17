void GDataCache::GetResourceIdsOfBacklog(
    std::vector<std::string>* to_fetch,
    std::vector<std::string>* to_upload) {
  AssertOnSequencedWorkerPool();
  DCHECK(to_fetch);
  DCHECK(to_upload);

  metadata_->Iterate(base::Bind(&CollectBacklog, to_fetch, to_upload));
}
