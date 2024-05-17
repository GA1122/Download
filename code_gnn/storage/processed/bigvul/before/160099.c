int BackendImpl::SyncDoomEntriesSince(const base::Time initial_time) {
  DCHECK_NE(net::APP_CACHE, cache_type_);
  if (disabled_)
    return net::ERR_FAILED;

  stats_.OnEvent(Stats::DOOM_RECENT);
  for (;;) {
    std::unique_ptr<Rankings::Iterator> iterator(new Rankings::Iterator());
    scoped_refptr<EntryImpl> entry = OpenNextEntryImpl(iterator.get());
    if (!entry)
      return net::OK;

    if (initial_time > entry->GetLastUsed()) {
      entry = nullptr;
      SyncEndEnumeration(std::move(iterator));
      return net::OK;
    }

    entry->DoomImpl();
    entry = nullptr;
    SyncEndEnumeration(
        std::move(iterator));   
  }
}
