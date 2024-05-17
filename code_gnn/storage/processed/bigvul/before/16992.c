int WebContext::maxCacheSizeHint() const {
  if (IsInitialized()) {
    return context_->GetMaxCacheSizeHint();
  }

  return construct_props_->max_cache_size_hint;
}
