void OfflinePageModelImpl::OnDeleteArchiveFilesDone(
    const std::vector<int64_t>& offline_ids,
    const DeletePageCallback& callback,
    bool success) {
  if (!success) {
    InformDeletePageDone(callback, DeletePageResult::DEVICE_FAILURE);
    return;
  }

  store_->RemoveOfflinePages(
      offline_ids, base::Bind(&OfflinePageModelImpl::OnRemoveOfflinePagesDone,
                              weak_ptr_factory_.GetWeakPtr(), callback));
}
