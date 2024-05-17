void WebContext::setMaxCacheSizeHint(int size) {
  DCHECK(!IsInitialized());
  construct_props_->max_cache_size_hint = size;
}
