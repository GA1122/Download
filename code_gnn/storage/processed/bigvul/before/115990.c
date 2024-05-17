void ExtensionGlobalError::AddBlacklistedExtension(const std::string& id) {
  blacklisted_extension_ids_->insert(id);
}
