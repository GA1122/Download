void SyncBackendHost::HandleInitializationCompletedOnFrontendLoop(
    const WeakHandle<JsBackend>& js_backend, bool success) {
  DCHECK_NE(NOT_ATTEMPTED, initialization_state_);
  if (!frontend_)
    return;

  if (initialization_state_ == CREATING_SYNC_MANAGER)
    initialization_state_ = NOT_INITIALIZED;

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  if (!success) {
    initialization_state_ = NOT_INITIALIZED;
    frontend_->OnBackendInitialized(WeakHandle<JsBackend>(), false);
    return;
  }

  CHECK(sync_prefs_.get());
  if (sync_prefs_->HasSyncSetupCompleted() &&
      initialization_state_ < DOWNLOADING_NIGORI) {
    initialization_state_ = DOWNLOADING_NIGORI;
  }

  switch (initialization_state_) {
    case NOT_INITIALIZED:
      initialization_state_ = DOWNLOADING_NIGORI;
      ConfigureDataTypes(
          sync_api::CONFIGURE_REASON_NEW_CLIENT,
          syncable::ModelTypeSet(),
          syncable::ModelTypeSet(),
          WITH_NIGORI,
          base::Bind(
              &SyncBackendHost::
                  HandleNigoriConfigurationCompletedOnFrontendLoop,
              weak_ptr_factory_.GetWeakPtr(), js_backend),
          base::Bind(&SyncBackendHost::OnNigoriDownloadRetry,
                     weak_ptr_factory_.GetWeakPtr()));
      break;
    case DOWNLOADING_NIGORI:
      initialization_state_ = REFRESHING_NIGORI;
      RefreshNigori(
          base::Bind(
              &SyncBackendHost::
                  HandleInitializationCompletedOnFrontendLoop,
              weak_ptr_factory_.GetWeakPtr(), js_backend, true));
      break;
    case REFRESHING_NIGORI:
      initialization_state_ = INITIALIZED;
      AddExperimentalTypes();
      frontend_->OnBackendInitialized(js_backend, true);
      break;
    default:
      NOTREACHED();
  }
}
