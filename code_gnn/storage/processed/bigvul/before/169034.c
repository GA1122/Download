void OfflinePageModelImpl::CacheLoadedData(
    const std::vector<OfflinePageItem>& offline_pages) {
  offline_pages_.clear();
  for (const auto& offline_page : offline_pages)
    offline_pages_[offline_page.offline_id] = offline_page;
}
