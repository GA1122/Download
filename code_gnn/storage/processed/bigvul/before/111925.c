std::string ProfileSyncService::QuerySyncStatusSummary() {
  if (unrecoverable_error_detected_) {
    return "Unrecoverable error detected";
  } else if (!backend_.get()) {
    return "Syncing not enabled";
  } else if (backend_.get() && !HasSyncSetupCompleted()) {
    return "First time sync setup incomplete";
  } else if (backend_.get() && HasSyncSetupCompleted() &&
             data_type_manager_.get() &&
             data_type_manager_->state() != DataTypeManager::CONFIGURED) {
    return "Datatypes not fully initialized";
  } else if (ShouldPushChanges()) {
    return "Sync service initialized";
  } else {
    return "Status unknown: Internal error?";
  }
}
