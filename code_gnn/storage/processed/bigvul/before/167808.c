void DownloadRequestLimiter::CanDownload(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const GURL& url,
    const std::string& request_method,
    const Callback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  content::WebContents* originating_contents = web_contents_getter.Run();
  if (!originating_contents) {
    callback.Run(false);
    return;
  }

  if (!originating_contents->GetDelegate()) {
    callback.Run(false);
    return;
  }

  base::Callback<void(bool)> can_download_callback = base::Bind(
      &DownloadRequestLimiter::OnCanDownloadDecided, factory_.GetWeakPtr(),
      web_contents_getter, request_method, callback);

  originating_contents->GetDelegate()->CanDownload(url, request_method,
                                                   can_download_callback);
}
