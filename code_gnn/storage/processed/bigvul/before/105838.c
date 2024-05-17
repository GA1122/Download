STDMETHODIMP UrlmonUrlRequest::OnStartBinding(DWORD reserved,
                                              IBinding* binding) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  binding_ = binding;
  if (pending_) {
    response_headers_ = GetHttpHeadersFromBinding(binding_);
    DCHECK(!response_headers_.empty());
  }
  return S_OK;
}
