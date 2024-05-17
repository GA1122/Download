scoped_refptr<EntryImpl> BackendImpl::GetEnumeratedEntry(
    CacheRankingsBlock* next,
    Rankings::List list) {
  if (!next || disabled_)
    return NULL;

  scoped_refptr<EntryImpl> entry;
  int rv = NewEntry(Addr(next->Data()->contents), &entry);
  if (rv) {
    STRESS_NOTREACHED();
    rankings_.Remove(next, list, false);
    if (rv == ERR_INVALID_ADDRESS) {
      DeleteBlock(next->address(), true);
    }
    return NULL;
  }

  if (entry->dirty()) {
    InternalDoomEntry(entry.get());
    return NULL;
  }

  if (!entry->Update()) {
    STRESS_NOTREACHED();
    return NULL;
  }


  entry->GetKey();

  return entry;
}
