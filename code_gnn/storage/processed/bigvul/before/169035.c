void OfflinePageModelImpl::CheckMetadataConsistencyForArchivePaths(
    const std::set<base::FilePath>& archive_paths) {
  DeleteTemporaryPagesInAbandonedCacheDir();
  DeletePagesMissingArchiveFile(archive_paths);
  DeleteOrphanedArchives(archive_paths);
}
