void OfflinePageModelImpl::OnMarkPageAccesseDone(
    const OfflinePageItem& offline_page_item,
    std::unique_ptr<OfflinePagesUpdateResult> result) {
  if (result->updated_items.size() > 0)
    offline_pages_[offline_page_item.offline_id] = offline_page_item;

}
