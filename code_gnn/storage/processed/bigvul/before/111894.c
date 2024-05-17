void ProfileSyncService::InitializeBackend(bool delete_stale_data) {
  if (!backend_.get()) {
    NOTREACHED();
    return;
  }

  syncable::ModelTypeSet initial_types;
  if (HasSyncSetupCompleted()) {
    initial_types = GetPreferredDataTypes();
  }

  SyncCredentials credentials = GetCredentials();

  scoped_refptr<net::URLRequestContextGetter> request_context_getter(
      profile_->GetRequestContext());

  if (delete_stale_data)
    ClearStaleErrors();

  backend_unrecoverable_error_handler_.reset(
    new browser_sync::BackendUnrecoverableErrorHandler(
        MakeWeakHandle(AsWeakPtr())));

  backend_->Initialize(
      this,
      MakeWeakHandle(sync_js_controller_.AsWeakPtr()),
      sync_service_url_,
      initial_types,
      credentials,
      delete_stale_data,
      backend_unrecoverable_error_handler_.get(),
      &browser_sync::ChromeReportUnrecoverableError);
}
