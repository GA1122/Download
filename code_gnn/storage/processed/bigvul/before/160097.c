int BackendImpl::SyncDoomAllEntries() {
  if (disabled_)
    return net::ERR_FAILED;

  ReportError(ERR_CACHE_DOOMED);
  stats_.OnEvent(Stats::DOOM_CACHE);
  if (!num_refs_) {
    RestartCache(false);
    return disabled_ ? net::ERR_FAILED : net::OK;
  } else {
    if (disabled_)
      return net::ERR_FAILED;

    eviction_.TrimCache(true);
    return net::OK;
  }
}
