bool UrlmonUrlRequest::Start() {
  DVLOG(1) << __FUNCTION__ << me() << url();
  DCHECK(thread_ == 0 || thread_ == base::PlatformThread::CurrentId());
  thread_ = base::PlatformThread::CurrentId();
  status_.Start();
  socket_address_ = net::HostPortPair::FromURL(GURL(url()));
  scoped_refptr<UrlmonUrlRequest> ref(this);
  HRESULT hr = StartAsyncDownload();
  if (FAILED(hr) && status_.get_state() != UrlmonUrlRequest::Status::DONE) {
    status_.Done();
    status_.set_result(net::URLRequestStatus::FAILED, HresultToNetError(hr));
    NotifyDelegateAndDie();
  }
  return true;
}
