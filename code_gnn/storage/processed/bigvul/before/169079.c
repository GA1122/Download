void OfflinePageModelImpl::OnDeleteOrphanedArchivesDone(
    const std::vector<base::FilePath>& archives,
    bool success) {
  UMA_HISTOGRAM_COUNTS("OfflinePages.Consistency.OrphanedArchivesCount",
                       static_cast<int32_t>(archives.size()));
  UMA_HISTOGRAM_BOOLEAN("OfflinePages.Consistency.DeleteOrphanedArchivesResult",
                        success);
}
