void CheckCanDownload(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const GURL& url,
    const std::string& request_method,
    CanDownloadCallback can_download_cb) {
  DownloadRequestLimiter* limiter =
      g_browser_process->download_request_limiter();
  if (limiter) {
    DownloadRequestLimiter::Callback cb =
        base::Bind(&CheckDownloadComplete, base::Passed(&can_download_cb));
    limiter->CanDownload(web_contents_getter, url, request_method, cb);
  }
}
