int BackendImpl::Init(const CompletionCallback& callback) {
  background_queue_.Init(callback);
  return net::ERR_IO_PENDING;
}
