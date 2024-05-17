void CollectLocalStorageUsage(std::vector<StorageUsageInfo>* out_info,
                              base::OnceClosure done_callback,
                              const std::vector<StorageUsageInfo>& in_info) {
  out_info->insert(out_info->end(), in_info.begin(), in_info.end());
  std::move(done_callback).Run();
}
