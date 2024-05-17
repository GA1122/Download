void UrlmonUrlRequest::NotifyDelegateAndDie() {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DVLOG(1) << __FUNCTION__ << me();

  PluginUrlRequestDelegate* delegate = delegate_;
  delegate_ = NULL;
  ReleaseBindings();
  TerminateTransaction();
  if (delegate && id() != -1) {
    net::URLRequestStatus result = status_.get_result();
    delegate->OnResponseEnd(id(), result);
  } else {
    DLOG(WARNING) << __FUNCTION__ << me() << "no delegate";
  }
}
