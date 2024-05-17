void AppCacheHost::SetSwappableCache(AppCacheGroup* group) {
  if (!group) {
    swappable_cache_ = NULL;
  } else {
    AppCache* new_cache = group->newest_complete_cache();
    if (new_cache != associated_cache_.get())
      swappable_cache_ = new_cache;
    else
      swappable_cache_ = NULL;
  }
}
