UrlIndex::~UrlIndex() {
#if DCHECK_IS_ON()
  auto dcheck_has_one_ref = [](const UrlDataMap::value_type& entry) {
    DCHECK(entry.second->HasOneRef());
  };
  std::for_each(indexed_data_.begin(), indexed_data_.end(), dcheck_has_one_ref);
#endif
}
