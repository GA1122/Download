int BackendImpl::DoomAllEntries(const CompletionCallback& callback) {
  DCHECK(!callback.is_null());
  background_queue_.DoomAllEntries(callback);
  return net::ERR_IO_PENDING;
}
