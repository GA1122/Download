void CursorImpl::PrefetchReset(
    int32_t used_prefetches,
    int32_t unused_prefetches,
    const std::vector<std::string>& unused_blob_uuids) {
  for (const auto& uuid : unused_blob_uuids)
    dispatcher_host_->DropBlobData(uuid);

  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::PrefetchReset, base::Unretained(helper_),
                 used_prefetches, unused_prefetches));
}
