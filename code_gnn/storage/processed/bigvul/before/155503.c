DataReductionProxySettings::GetProxyRequestHeaders() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return proxy_request_headers_;
}
