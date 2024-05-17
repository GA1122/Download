void OfflinePageModelImpl::GetPagesMatchingQueryWhenLoadDone(
    std::unique_ptr<OfflinePageModelQuery> query,
    const MultipleOfflinePageItemCallback& callback) {
  DCHECK(query);
  DCHECK(is_loaded_);

  MultipleOfflinePageItemResult offline_pages_result;

  for (const auto& id_page_pair : offline_pages_) {
    if (query->Matches(id_page_pair.second))
      offline_pages_result.emplace_back(id_page_pair.second);
  }

  callback.Run(offline_pages_result);
}
