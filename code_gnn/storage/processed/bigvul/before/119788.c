void NavigationControllerImpl::InsertEntriesFrom(
    const NavigationControllerImpl& source,
    int max_index) {
  DCHECK_LE(max_index, source.GetEntryCount());
  size_t insert_index = 0;
  for (int i = 0; i < max_index; i++) {
    if (source.entries_[i].get()->GetPageType() !=
        PAGE_TYPE_INTERSTITIAL) {
      entries_.insert(entries_.begin() + insert_index++,
                      linked_ptr<NavigationEntryImpl>(
                          new NavigationEntryImpl(*source.entries_[i])));
    }
  }
}
