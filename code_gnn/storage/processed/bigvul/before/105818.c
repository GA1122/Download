void UrlmonUrlRequestManager::EndRequest(int request_id) {
  DVLOG(1) << __FUNCTION__ << " id: " << request_id;
  DCHECK_EQ(0, calling_delegate_);
  scoped_refptr<UrlmonUrlRequest> request = LookupRequest(request_id);
  if (request) {
    request_map_.erase(request_id);
    request->Stop();
  } else {
    DLOG(ERROR) << __FUNCTION__ << " no request found for " << request_id;
  }
}
