int BackendImpl::DoomEntry(const std::string& key,
                           const CompletionCallback& callback) {
  DCHECK(!callback.is_null());
  background_queue_.DoomEntry(key, callback);
  return net::ERR_IO_PENDING;
}
