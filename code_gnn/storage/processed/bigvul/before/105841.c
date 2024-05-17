void UrlmonUrlRequestManager::ReadRequest(int request_id, int bytes_to_read) {
  DVLOG(1) << __FUNCTION__ << " id: " << request_id;
  DCHECK_EQ(0, calling_delegate_);
  scoped_refptr<UrlmonUrlRequest> request = LookupRequest(request_id);
  if (request)
    request->Read(bytes_to_read);
}
