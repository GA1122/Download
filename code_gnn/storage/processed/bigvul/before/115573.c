void DNSWatchDelegate::OnFilePathChanged(const FilePath& path) {
  callback_.Run();
}
