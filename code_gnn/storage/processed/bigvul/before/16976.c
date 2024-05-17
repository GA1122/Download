void WebContext::clearTemporarySavedPermissionStatuses() {
  if (!context_.get()) {
    return;
  }

  context_->GetTemporarySavedPermissionContext()->Clear();
  if (!context_->HasOffTheRecordContext()) {
    return;
  }

  context_->GetOffTheRecordContext()
      ->GetTemporarySavedPermissionContext()
      ->Clear();
}
