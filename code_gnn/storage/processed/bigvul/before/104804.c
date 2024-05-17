bool ExtensionService::IsExtensionEnabled(
    const std::string& extension_id) const {
  return
      extension_prefs_->GetExtensionState(extension_id) == Extension::ENABLED;
}
