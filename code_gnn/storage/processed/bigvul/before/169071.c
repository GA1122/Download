const std::vector<int64_t> OfflinePageModelImpl::MaybeGetOfflineIdsForClientId(
    const ClientId& client_id) const {
  DCHECK(is_loaded_);
  std::vector<int64_t> results;

  for (const auto& id_page_pair : offline_pages_) {
    if (id_page_pair.second.client_id == client_id)
      results.push_back(id_page_pair.second.offline_id);
  }
  return results;
}
