int BackendImpl::DoomEntriesBetween(const base::Time initial_time,
                                    const base::Time end_time,
                                    const CompletionCallback& callback) {
  DCHECK(!callback.is_null());
  background_queue_.DoomEntriesBetween(initial_time, end_time, callback);
  return net::ERR_IO_PENDING;
}
