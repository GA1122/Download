void IndexedDBDatabase::RequestComplete(ConnectionRequest* request) {
  DCHECK_EQ(request, active_request_.get());
  active_request_.reset();

  if (!pending_requests_.empty())
    ProcessRequestQueue();
}
