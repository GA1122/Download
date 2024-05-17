void OfflinePageModelImpl::InformDeletePageDone(
    const DeletePageCallback& callback,
    DeletePageResult result) {
  UMA_HISTOGRAM_ENUMERATION("OfflinePages.DeletePageResult",
                            static_cast<int>(result),
                            static_cast<int>(DeletePageResult::RESULT_COUNT));
  archive_manager_->GetStorageStats(
      base::Bind(&ReportStorageHistogramsAfterDelete));
  if (!callback.is_null())
    callback.Run(result);
}
