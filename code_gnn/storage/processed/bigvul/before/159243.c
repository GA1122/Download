void DownloadManagerImpl::DownloadUrl(
    std::unique_ptr<DownloadUrlParameters> params) {
  if (params->post_id() >= 0) {
    DCHECK(params->prefer_cache());
    DCHECK_EQ("POST", params->method());
  }

  BeginDownloadInternal(std::move(params), content::DownloadItem::kInvalidId);
}
