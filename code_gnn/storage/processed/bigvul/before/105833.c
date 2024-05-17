void UrlmonUrlRequestManager::OnReadComplete(int request_id,
                                             const std::string& data) {
  DCHECK_NE(request_id, -1);
  DVLOG(1) << __FUNCTION__ << " id: " << request_id;
  DCHECK(LookupRequest(request_id) != NULL);
  ++calling_delegate_;
  delegate_->OnReadComplete(request_id, data);
  --calling_delegate_;
  DVLOG(1) << __FUNCTION__ << " done id: " << request_id;
}
