scoped_refptr<EntryImpl> BackendImpl::CreateEntryImpl(const std::string& key) {
  if (disabled_ || key.empty())
    return NULL;

  TimeTicks start = TimeTicks::Now();
  uint32_t hash = base::Hash(key);
  Trace("Create hash 0x%x", hash);

  scoped_refptr<EntryImpl> parent;
  Addr entry_address(data_->table[hash & mask_]);
  if (entry_address.is_initialized()) {
    bool error;
    scoped_refptr<EntryImpl> old_entry =
        MatchEntry(key, hash, false, Addr(), &error);
    if (old_entry)
      return ResurrectEntry(std::move(old_entry));

    parent = MatchEntry(key, hash, true, Addr(), &error);
    DCHECK(!error);
    if (!parent && data_->table[hash & mask_]) {
      NOTREACHED();
      return NULL;
    }
  }


  int num_blocks = EntryImpl::NumBlocksForEntry(key.size());
  if (!block_files_.CreateBlock(BLOCK_256, num_blocks, &entry_address)) {
    LOG(ERROR) << "Create entry failed " << key.c_str();
    stats_.OnEvent(Stats::CREATE_ERROR);
    return NULL;
  }

  Addr node_address(0);
  if (!block_files_.CreateBlock(RANKINGS, 1, &node_address)) {
    block_files_.DeleteBlock(entry_address, false);
    LOG(ERROR) << "Create entry failed " << key.c_str();
    stats_.OnEvent(Stats::CREATE_ERROR);
    return NULL;
  }

  scoped_refptr<EntryImpl> cache_entry(
      new EntryImpl(this, entry_address, false));
  IncreaseNumRefs();

  if (!cache_entry->CreateEntry(node_address, key, hash)) {
    block_files_.DeleteBlock(entry_address, false);
    block_files_.DeleteBlock(node_address, false);
    LOG(ERROR) << "Create entry failed " << key.c_str();
    stats_.OnEvent(Stats::CREATE_ERROR);
    return NULL;
  }

  cache_entry->BeginLogging(net_log_, true);

  open_entries_[entry_address.value()] = cache_entry.get();

  cache_entry->entry()->Store();
  cache_entry->rankings()->Store();
  IncreaseNumEntries();
  entry_count_++;

  if (parent.get()) {
    parent->SetNextAddress(entry_address);
  } else {
    data_->table[hash & mask_] = entry_address.value();
  }

  eviction_.OnCreateEntry(cache_entry.get());

  CACHE_UMA(AGE_MS, "CreateTime", 0, start);
  stats_.OnEvent(Stats::CREATE_HIT);
  Trace("create entry hit ");
  FlushIndex();
  return cache_entry;
}
