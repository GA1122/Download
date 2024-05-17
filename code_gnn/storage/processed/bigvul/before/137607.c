void DownloadController::AcquireFileAccessPermission(
    WebContents* web_contents,
    const DownloadControllerBase::AcquireFileAccessPermissionCallback& cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(web_contents);

  ui::ViewAndroid* view_android =
      ViewAndroidHelper::FromWebContents(web_contents)->GetViewAndroid();
  if (!view_android) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE, base::Bind(cb, false));
    return;
  }
  ui::WindowAndroid* window_android = view_android->GetWindowAndroid();
  if (window_android && HasFileAccessPermission(window_android)) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE, base::Bind(cb, true));
    return;
  }
  intptr_t callback_id = reinterpret_cast<intptr_t>(
      new DownloadControllerBase::AcquireFileAccessPermissionCallback(cb));
  ChromeDownloadDelegate::FromWebContents(web_contents)->
      RequestFileAccess(callback_id);
}
