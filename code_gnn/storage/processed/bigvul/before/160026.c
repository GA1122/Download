int BackendImpl::DoomEntriesSince(const base::Time initial_time,
                                  const CompletionCallback& callback) {
  DCHECK(!callback.is_null());
  background_queue_.DoomEntriesSince(initial_time, callback);
  return net::ERR_IO_PENDING;
}
