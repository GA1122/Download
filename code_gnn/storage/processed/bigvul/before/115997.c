ExtensionGlobalError::ExtensionGlobalError(
      base::WeakPtr<ExtensionService> extension_service)
    : current_browser_(NULL),
      should_delete_self_on_close_(true),
      extension_service_(extension_service),
      external_extension_ids_(new ExtensionIdSet),
      blacklisted_extension_ids_(new ExtensionIdSet),
      orphaned_extension_ids_(new ExtensionIdSet) {
}
