scoped_refptr<EntryImpl> BackendImpl::MatchEntry(const std::string& key,
                                                 uint32_t hash,
                                                 bool find_parent,
                                                 Addr entry_addr,
                                                 bool* match_error) {
  Addr address(data_->table[hash & mask_]);
  scoped_refptr<EntryImpl> cache_entry, parent_entry;
  bool found = false;
  std::set<CacheAddr> visited;
  *match_error = false;

  for (;;) {
    if (disabled_)
      break;

    if (visited.find(address.value()) != visited.end()) {
      Trace("Hash collision loop 0x%x", address.value());
      address.set_value(0);
      parent_entry->SetNextAddress(address);
    }
    visited.insert(address.value());

    if (!address.is_initialized()) {
      if (find_parent)
        found = true;
      break;
    }

    int error = NewEntry(address, &cache_entry);
    if (error || cache_entry->dirty()) {
      Addr child(0);
      if (!error)
        child.set_value(cache_entry->GetNextAddress());

      if (parent_entry.get()) {
        parent_entry->SetNextAddress(child);
        parent_entry = NULL;
      } else {
        data_->table[hash & mask_] = child.value();
      }

      Trace("MatchEntry dirty %d 0x%x 0x%x", find_parent, entry_addr.value(),
            address.value());

      if (!error) {
        DestroyInvalidEntry(cache_entry.get());
        cache_entry = NULL;
      } else {
        Trace("NewEntry failed on MatchEntry 0x%x", address.value());
      }

      address.set_value(data_->table[hash & mask_]);
      visited.clear();
      continue;
    }

    DCHECK_EQ(hash & mask_, cache_entry->entry()->Data()->hash & mask_);
    if (cache_entry->IsSameEntry(key, hash)) {
      if (!cache_entry->Update())
        cache_entry = NULL;
      found = true;
      if (find_parent && entry_addr.value() != address.value()) {
        Trace("Entry not on the index 0x%x", address.value());
        *match_error = true;
        parent_entry = NULL;
      }
      break;
    }
    if (!cache_entry->Update())
      cache_entry = NULL;
    parent_entry = cache_entry;
    cache_entry = NULL;
    if (!parent_entry.get())
      break;

    address.set_value(parent_entry->GetNextAddress());
  }

  if (parent_entry.get() && (!find_parent || !found))
    parent_entry = NULL;

  if (find_parent && entry_addr.is_initialized() && !cache_entry.get()) {
    *match_error = true;
    parent_entry = NULL;
  }

  if (cache_entry.get() && (find_parent || !found))
    cache_entry = NULL;

  FlushIndex();

  return find_parent ? std::move(parent_entry) : std::move(cache_entry);
}
