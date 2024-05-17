void OfflinePageModelImpl::OnDeletePagesMissingArchiveFileDone(
    const std::vector<int64_t>& offline_ids,
    DeletePageResult result) {
  UMA_HISTOGRAM_COUNTS("OfflinePages.Consistency.PagesMissingArchiveFileCount",
                       static_cast<int32_t>(offline_ids.size()));
  UMA_HISTOGRAM_ENUMERATION(
      "OfflinePages.Consistency.DeletePagesMissingArchiveFileResult",
      static_cast<int>(result),
      static_cast<int>(DeletePageResult::RESULT_COUNT));
}
