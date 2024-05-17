scoped_refptr<UrlmonUrlRequest> UrlmonUrlRequestManager::LookupRequest(
    int request_id) {
  RequestMap::iterator it = request_map_.find(request_id);
  if (request_map_.end() != it)
    return it->second;
  DLOG(ERROR) << __FUNCTION__ << " no request found for " << request_id;
  return NULL;
}
