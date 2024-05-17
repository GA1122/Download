void UrlmonUrlRequestManager::OnResponseEnd(
    int request_id,
    const net::URLRequestStatus& status) {
  DCHECK_NE(request_id, -1);
  DVLOG(1) << __FUNCTION__;
  DCHECK(status.status() != net::URLRequestStatus::CANCELED);
  RequestMap::size_type n = request_map_.erase(request_id);
  if (n != 1u) {
    DLOG(WARNING) << __FUNCTION__
                  << " Failed to find request id:"
                  << request_id;
  }
  ++calling_delegate_;
  delegate_->OnResponseEnd(request_id, status);
  --calling_delegate_;
}
