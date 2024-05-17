void OfflinePageModelTaskified::OnAddPageForSavePageDone(
    const SavePageCallback& callback,
    const OfflinePageItem& page_attempted,
    AddPageResult add_page_result,
    int64_t offline_id) {
  SavePageResult save_page_result =
      AddPageResultToSavePageResult(add_page_result);
  InformSavePageDone(callback, save_page_result, page_attempted);
  if (save_page_result == SavePageResult::SUCCESS)
    RemovePagesMatchingUrlAndNamespace(page_attempted);
  PostClearCachedPagesTask(false  );
}
