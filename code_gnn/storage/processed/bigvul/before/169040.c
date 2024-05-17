void OfflinePageModelImpl::DeletePages(
    const DeletePageCallback& callback,
    const MultipleOfflinePageItemResult& pages) {
  DCHECK(is_loaded_);

  std::vector<int64_t> offline_ids;
  for (auto& page : pages)
    offline_ids.emplace_back(page.offline_id);

  DoDeletePagesByOfflineId(offline_ids, callback);
}
