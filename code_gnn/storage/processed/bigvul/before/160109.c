void BackendImpl::UpdateRank(EntryImpl* entry, bool modified) {
  if (read_only_ || (!modified && cache_type() == net::SHADER_CACHE))
    return;
  eviction_.UpdateRank(entry, modified);
}
