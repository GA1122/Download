int BackendImpl::CalculateSizeOfAllEntries(const CompletionCallback& callback) {
  DCHECK(!callback.is_null());
  background_queue_.CalculateSizeOfAllEntries(callback);
  return net::ERR_IO_PENDING;
}
