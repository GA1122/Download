void ResourceDispatcherHost::StartReading(URLRequest* request) {
  int bytes_read = 0;
  if (Read(request, &bytes_read)) {
    OnReadCompleted(request, bytes_read);
  } else if (!request->status().is_io_pending()) {
    DCHECK(!InfoForRequest(request)->is_paused());
    OnResponseCompleted(request);
  }
}
