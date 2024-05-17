void DownloadManagerDelegate::CheckDownloadAllowed(
    const ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const GURL& url,
    const std::string& request_method,
    CheckDownloadAllowedCallback check_download_allowed_cb) {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(std::move(check_download_allowed_cb), true));
}
