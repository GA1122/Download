void DownloadRequestLimiter::OnCanDownloadDecided(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const std::string& request_method,
    const Callback& orig_callback,
    bool allow) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  content::WebContents* originating_contents = web_contents_getter.Run();
  if (!originating_contents || !allow) {
    orig_callback.Run(false);
    return;
  }

  CanDownloadImpl(originating_contents, request_method, orig_callback);
}
