void OfflinePageModelImpl::DoDeleteCachedPagesByURLPredicate(
    const UrlPredicate& predicate,
    const DeletePageCallback& callback) {
  DCHECK(is_loaded_);

  std::vector<int64_t> offline_ids;
  for (const auto& id_page_pair : offline_pages_) {
    if (policy_controller_->IsRemovedOnCacheReset(
            id_page_pair.second.client_id.name_space) &&
        predicate.Run(id_page_pair.second.url)) {
      offline_ids.push_back(id_page_pair.first);
    }
  }
  DoDeletePagesByOfflineId(offline_ids, callback);
}
