void ChromeDownloadManagerDelegate::CheckDownloadAllowed(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const GURL& url,
    const std::string& request_method,
    content::CheckDownloadAllowedCallback check_download_allowed_cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  CanDownloadCallback cb = base::BindOnce(
      &ChromeDownloadManagerDelegate::OnCheckDownloadAllowedComplete,
      weak_ptr_factory_.GetWeakPtr(), std::move(check_download_allowed_cb));
#if defined(OS_ANDROID)
  DownloadControllerBase::Get()->AcquireFileAccessPermission(
      web_contents_getter,
      base::Bind(&OnAcquireFileAccessPermissionDone, web_contents_getter, url,
                 request_method, base::Passed(&cb)));
#else
  CheckCanDownload(web_contents_getter, url, request_method, std::move(cb));
#endif
}
