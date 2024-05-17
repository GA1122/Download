void ExtensionUpdateRequestSent(const std::string& id, bool success) {
  VLOG_IF(1, !success) << "Failed sending update request for " << id;
}
