bool BackendImpl::OpenFollowingEntryFromList(
    Rankings::List list,
    CacheRankingsBlock** from_entry,
    scoped_refptr<EntryImpl>* next_entry) {
  if (disabled_)
    return false;

  if (!new_eviction_ && Rankings::NO_USE != list)
    return false;

  Rankings::ScopedRankingsBlock rankings(&rankings_, *from_entry);
  CacheRankingsBlock* next_block = rankings_.GetNext(rankings.get(), list);
  Rankings::ScopedRankingsBlock next(&rankings_, next_block);
  *from_entry = NULL;

  *next_entry = GetEnumeratedEntry(next.get(), list);
  if (!*next_entry)
    return false;

  *from_entry = next.release();
  return true;
}
