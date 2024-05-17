void ExtensionInstallRequestSent(const std::string& id, bool success) {
  VLOG_IF(1, !success) << "Failed sending install request for " << id;
}
