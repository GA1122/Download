base::FilePath FileManagerBrowserTestBase::MaybeMountCrostini(
    const std::string& source_path,
    const std::vector<std::string>& mount_options) {
  GURL source_url(source_path);
  DCHECK(source_url.is_valid());
  if (source_url.scheme() != "sshfs") {
    return {};
  }
  CHECK(crostini_volume_->Mount(profile()));
  return crostini_volume_->mount_path();
}
