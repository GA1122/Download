void OfflinePageModelImpl::DeleteTemporaryPagesInAbandonedCacheDir() {
  std::vector<int64_t> offline_page_ids;
  for (const auto& id_page_pair : offline_pages_) {
    const OfflinePageItem& page = id_page_pair.second;
    if (policy_controller_->IsRemovedOnCacheReset(page.client_id.name_space) &&
        !archive_manager_->GetTemporaryArchivesDir().IsParent(page.file_path))
      offline_page_ids.push_back(id_page_pair.first);
  }

  if (offline_page_ids.empty())
    return;

  DeletePagesByOfflineId(offline_page_ids,
                         base::Bind([](const std::vector<int64_t>& offline_ids,
                                       DeletePageResult result) {},
                                    offline_page_ids));
}
