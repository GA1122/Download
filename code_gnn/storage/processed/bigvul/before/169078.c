void OfflinePageModelImpl::OnDeleteOldPagesWithSameURL(
    DeletePageResult result) {
  PostClearStorageIfNeededTask(false  );
}
