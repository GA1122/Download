void ResourceDispatcherHostImpl::StartReading(net::URLRequest* request) {
  int bytes_read = 0;
  if (Read(request, &bytes_read)) {
    OnReadCompleted(request, bytes_read);
  } else if (!request->status().is_io_pending()) {
    DCHECK(!ResourceRequestInfoImpl::ForRequest(request)->is_paused());
    ResponseCompleted(request);
  }
}
