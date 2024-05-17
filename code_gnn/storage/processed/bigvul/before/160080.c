void BackendImpl::RemoveEntry(EntryImpl* entry) {
#if defined(NET_BUILD_STRESS_CACHE)
  NotLinked(entry);
#endif
  if (!new_eviction_)
    return;

  DCHECK_NE(ENTRY_NORMAL, entry->entry()->Data()->state);

  Trace("Remove entry 0x%p", entry);
  eviction_.OnDestroyEntry(entry);
  DecreaseNumEntries();
}
