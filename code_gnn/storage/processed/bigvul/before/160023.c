void BackendImpl::DestroyInvalidEntry(EntryImpl* entry) {
  LOG(WARNING) << "Destroying invalid entry.";
  Trace("Destroying invalid entry 0x%p", entry);

  entry->SetPointerForInvalidEntry(GetCurrentEntryId());

  eviction_.OnDoomEntry(entry);
  entry->InternalDoom();

  if (!new_eviction_)
    DecreaseNumEntries();
  stats_.OnEvent(Stats::INVALID_ENTRY);
}
