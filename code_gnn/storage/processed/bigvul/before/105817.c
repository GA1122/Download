void UrlmonUrlRequestManager::DownloadRequestInHost(int request_id) {
  DVLOG(1) << __FUNCTION__ << " " << request_id;
  if (IsWindow(notification_window_)) {
    scoped_refptr<UrlmonUrlRequest> request(LookupRequest(request_id));
    if (request) {
      UrlmonUrlRequest::TerminateBindCallback* callback = NewCallback(this,
          &UrlmonUrlRequestManager::BindTerminated);
      request->TerminateBind(callback);
    } else {
      NOTREACHED();
    }
  } else {
    NOTREACHED() << "Cannot handle download if we don't have anyone to hand it "
                    "to.";
  }
}
