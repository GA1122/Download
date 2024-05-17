void UrlmonUrlRequestManager::OnResponseStarted(int request_id,
    const char* mime_type, const char* headers, int size,
    base::Time last_modified, const std::string& redirect_url,
    int redirect_status, const net::HostPortPair& socket_address) {
  DCHECK_NE(request_id, -1);
  DVLOG(1) << __FUNCTION__;
  DCHECK(LookupRequest(request_id) != NULL);
  ++calling_delegate_;
  delegate_->OnResponseStarted(request_id, mime_type, headers, size,
      last_modified, redirect_url, redirect_status, socket_address);
  --calling_delegate_;
}
