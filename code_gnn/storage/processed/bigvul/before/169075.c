void OfflinePageModelImpl::OnAddSavedPageDone(
    const OfflinePageItem& offline_page,
    const SavePageCallback& callback,
    AddPageResult add_result,
    int64_t offline_id) {
  SavePageResult save_result;
  if (add_result == AddPageResult::SUCCESS) {
    save_result = SavePageResult::SUCCESS;
  } else if (add_result == AddPageResult::ALREADY_EXISTS) {
    save_result = SavePageResult::ALREADY_EXISTS;
  } else if (add_result == AddPageResult::STORE_FAILURE) {
    save_result = SavePageResult::STORE_FAILURE;
  } else {
    NOTREACHED();
    save_result = SavePageResult::STORE_FAILURE;
  }
  InformSavePageDone(callback, save_result, offline_page.client_id, offline_id);
  if (save_result == SavePageResult::SUCCESS) {
    DeleteExistingPagesWithSameURL(offline_page);
  } else {
    PostClearStorageIfNeededTask(false  );
  }
}
