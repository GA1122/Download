void DNSWatchDelegate::OnFilePathError(const FilePath& path) {
  LOG(ERROR) << "DNSWatchDelegate::OnFilePathError for " << path.value();
}
