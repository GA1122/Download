int BackendImpl::SyncDoomEntriesBetween(const base::Time initial_time,
                                        const base::Time end_time) {
  DCHECK_NE(net::APP_CACHE, cache_type_);
  if (end_time.is_null())
    return SyncDoomEntriesSince(initial_time);

  DCHECK(end_time >= initial_time);

  if (disabled_)
    return net::ERR_FAILED;

  scoped_refptr<EntryImpl> node;
  std::unique_ptr<Rankings::Iterator> iterator(new Rankings::Iterator());
  scoped_refptr<EntryImpl> next = OpenNextEntryImpl(iterator.get());
  if (!next)
    return net::OK;

  while (next) {
    node = std::move(next);
    next = OpenNextEntryImpl(iterator.get());

    if (node->GetLastUsed() >= initial_time &&
        node->GetLastUsed() < end_time) {
      node->DoomImpl();
    } else if (node->GetLastUsed() < initial_time) {
      next = NULL;
      SyncEndEnumeration(std::move(iterator));
    }
  }

  return net::OK;
}
