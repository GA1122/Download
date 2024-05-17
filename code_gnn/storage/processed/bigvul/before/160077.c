scoped_refptr<EntryImpl> BackendImpl::OpenNextEntryImpl(
    Rankings::Iterator* iterator) {
  if (disabled_)
    return NULL;

  const int kListsToSearch = 3;
  scoped_refptr<EntryImpl> entries[kListsToSearch];
  if (!iterator->my_rankings) {
    iterator->my_rankings = &rankings_;
    bool ret = false;

    for (int i = 0; i < kListsToSearch; i++) {
      ret |= OpenFollowingEntryFromList(static_cast<Rankings::List>(i),
                                        &iterator->nodes[i], &entries[i]);
    }
    if (!ret) {
      iterator->Reset();
      return NULL;
    }
  } else {
    for (int i = 0; i < kListsToSearch; i++) {
      if (iterator->list == i) {
        OpenFollowingEntryFromList(iterator->list, &iterator->nodes[i],
                                   &entries[i]);
      } else {
        entries[i] = GetEnumeratedEntry(iterator->nodes[i],
                                        static_cast<Rankings::List>(i));
      }
    }
  }

  int newest = -1;
  int oldest = -1;
  Time access_times[kListsToSearch];
  for (int i = 0; i < kListsToSearch; i++) {
    if (entries[i].get()) {
      access_times[i] = entries[i]->GetLastUsed();
      if (newest < 0) {
        DCHECK_LT(oldest, 0);
        newest = oldest = i;
        continue;
      }
      if (access_times[i] > access_times[newest])
        newest = i;
      if (access_times[i] < access_times[oldest])
        oldest = i;
    }
  }

  if (newest < 0 || oldest < 0) {
    iterator->Reset();
    return NULL;
  }

  scoped_refptr<EntryImpl> next_entry = entries[newest];
  iterator->list = static_cast<Rankings::List>(newest);
  return next_entry;
}
