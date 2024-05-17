void OfflinePageModelImpl::OnStorageCleared(size_t deleted_page_count,
                                            ClearStorageResult result) {
  UMA_HISTOGRAM_ENUMERATION("OfflinePages.ClearStorageResult",
                            static_cast<int>(result),
                            static_cast<int>(ClearStorageResult::RESULT_COUNT));
  if (deleted_page_count > 0) {
    UMA_HISTOGRAM_COUNTS("OfflinePages.ClearStorageBatchSize",
                         static_cast<int32_t>(deleted_page_count));
  }
}
