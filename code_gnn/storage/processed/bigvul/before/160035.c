int BackendImpl::FlushQueueForTest(const CompletionCallback& callback) {
  background_queue_.FlushQueue(callback);
  return net::ERR_IO_PENDING;
}
