int BackendImpl::NewEntry(Addr address, scoped_refptr<EntryImpl>* entry) {
  EntriesMap::iterator it = open_entries_.find(address.value());
  if (it != open_entries_.end()) {
    *entry = base::WrapRefCounted(it->second);
    return 0;
  }

  STRESS_DCHECK(block_files_.IsValid(address));

  if (!address.SanityCheckForEntry()) {
    LOG(WARNING) << "Wrong entry address.";
    STRESS_NOTREACHED();
    return ERR_INVALID_ADDRESS;
  }

  scoped_refptr<EntryImpl> cache_entry(
      new EntryImpl(this, address, read_only_));
  IncreaseNumRefs();
  *entry = NULL;

  TimeTicks start = TimeTicks::Now();
  if (!cache_entry->entry()->Load())
    return ERR_READ_FAILURE;

  if (IsLoaded()) {
    CACHE_UMA(AGE_MS, "LoadTime", 0, start);
  }

  if (!cache_entry->SanityCheck()) {
    LOG(WARNING) << "Messed up entry found.";
    STRESS_NOTREACHED();
    return ERR_INVALID_ENTRY;
  }

  STRESS_DCHECK(block_files_.IsValid(
                    Addr(cache_entry->entry()->Data()->rankings_node)));

  if (!cache_entry->LoadNodeAddress())
    return ERR_READ_FAILURE;

  if (!rankings_.SanityCheck(cache_entry->rankings(), false)) {
    STRESS_NOTREACHED();
    cache_entry->SetDirtyFlag(0);
    rankings_.SetContents(cache_entry->rankings(), 0);
  } else if (!rankings_.DataSanityCheck(cache_entry->rankings(), false)) {
    STRESS_NOTREACHED();
    cache_entry->SetDirtyFlag(0);
    rankings_.SetContents(cache_entry->rankings(), address.value());
  }

  if (!cache_entry->DataSanityCheck()) {
    LOG(WARNING) << "Messed up entry found.";
    cache_entry->SetDirtyFlag(0);
    cache_entry->FixForDelete();
  }

  cache_entry->SetDirtyFlag(GetCurrentEntryId());

  if (cache_entry->dirty()) {
    Trace("Dirty entry 0x%p 0x%x", reinterpret_cast<void*>(cache_entry.get()),
          address.value());
  }

  open_entries_[address.value()] = cache_entry.get();

  cache_entry->BeginLogging(net_log_, false);
  *entry = std::move(cache_entry);
  return 0;
}
