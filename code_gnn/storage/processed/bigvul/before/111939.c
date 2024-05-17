void ProfileSyncService::ShutdownImpl(bool sync_disabled) {
  base::Time shutdown_start_time = base::Time::Now();
  if (backend_.get())
    backend_->StopSyncingForShutdown();

  if (data_type_manager_.get()) {
    if (data_type_manager_->state() != DataTypeManager::STOPPED) {
      expect_sync_configuration_aborted_ = true;
      data_type_manager_->Stop();
    }

    registrar_.Remove(
        this,
        chrome::NOTIFICATION_SYNC_CONFIGURE_START,
        content::Source<DataTypeManager>(data_type_manager_.get()));
    registrar_.Remove(
        this,
        chrome::NOTIFICATION_SYNC_CONFIGURE_DONE,
        content::Source<DataTypeManager>(data_type_manager_.get()));
    registrar_.Remove(
        this,
        chrome::NOTIFICATION_SYNC_CONFIGURE_BLOCKED,
        content::Source<DataTypeManager>(data_type_manager_.get()));
    data_type_manager_.reset();
  }

  migrator_.reset();
  sync_js_controller_.AttachJsBackend(WeakHandle<JsBackend>());

  scoped_ptr<SyncBackendHost> doomed_backend(backend_.release());
  if (doomed_backend.get()) {
    doomed_backend->Shutdown(sync_disabled);

    doomed_backend.reset();
  }
  base::TimeDelta shutdown_time = base::Time::Now() - shutdown_start_time;
  UMA_HISTOGRAM_TIMES("Sync.Shutdown.BackendDestroyedTime", shutdown_time);

  weak_factory_.InvalidateWeakPtrs();

  expect_sync_configuration_aborted_ = false;
  is_auth_in_progress_ = false;
  backend_initialized_ = false;
  cached_passphrase_.clear();
  encryption_pending_ = false;
  encrypt_everything_ = false;
  encrypted_types_ = browser_sync::Cryptographer::SensitiveTypes();
  passphrase_required_reason_ = sync_api::REASON_PASSPHRASE_NOT_REQUIRED;
  last_auth_error_ = GoogleServiceAuthError::None();

  if (sync_global_error_.get()) {
    GlobalErrorServiceFactory::GetForProfile(profile_)->RemoveGlobalError(
        sync_global_error_.get());
    RemoveObserver(sync_global_error_.get());
    sync_global_error_.reset(NULL);
  }
}
