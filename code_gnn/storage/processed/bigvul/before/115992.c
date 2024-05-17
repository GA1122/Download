void ExtensionGlobalError::AddOrphanedExtension(const std::string& id) {
  orphaned_extension_ids_->insert(id);
}
