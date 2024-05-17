void OfflinePageModelImpl::InformSavePageDone(const SavePageCallback& callback,
                                              SavePageResult result,
                                              const ClientId& client_id,
                                              int64_t offline_id) {
  ReportSavePageResultHistogramAfterSave(client_id, result);
  archive_manager_->GetStorageStats(
      base::Bind(&ReportStorageHistogramsAfterSave));
  if (result == SavePageResult::ARCHIVE_CREATION_FAILED)
    archive_manager_->EnsureArchivesDirCreated(base::Bind([]() {}));
  callback.Run(result, offline_id);
}
