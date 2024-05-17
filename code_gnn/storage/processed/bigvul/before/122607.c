void Extension::AddInstallWarnings(
    const std::vector<InstallWarning>& new_warnings) {
  install_warnings_.insert(install_warnings_.end(),
                           new_warnings.begin(), new_warnings.end());
}
