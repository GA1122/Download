void ResourcePrefetchPredictor::CreateCaches(
    std::unique_ptr<RedirectDataMap> host_redirect_data,
    std::unique_ptr<OriginDataMap> origin_data) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_EQ(INITIALIZING, initialization_state_);

  DCHECK(host_redirect_data);
  DCHECK(origin_data);

  host_redirect_data_ = std::move(host_redirect_data);
  origin_data_ = std::move(origin_data);

  ConnectToHistoryService();
}
