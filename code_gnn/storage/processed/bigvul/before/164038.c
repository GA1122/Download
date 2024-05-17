net::URLRequestContextGetter* DownloadManagerImpl::GetURLRequestContextGetter(
    const download::DownloadCreateInfo& info) {
  StoragePartition* storage_partition = GetStoragePartition(
      browser_context_, info.render_process_id, info.render_frame_id);
  return storage_partition ? storage_partition->GetURLRequestContext()
                           : nullptr;
}
