void OfflinePageModelTaskified::CreateArchivesDirectoryIfNeeded() {
  archive_manager_->EnsureArchivesDirCreated(base::Bind([]() {}));
}
