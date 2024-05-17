void SyncBackendHost::Initialize(
    SyncFrontend* frontend,
    const WeakHandle<JsEventHandler>& event_handler,
    const GURL& sync_service_url,
    syncable::ModelTypeSet initial_types,
    const SyncCredentials& credentials,
    bool delete_sync_data_folder,
    UnrecoverableErrorHandler* unrecoverable_error_handler,
    ReportUnrecoverableErrorFunction report_unrecoverable_error_function) {
  if (!sync_thread_.Start())
    return;

  frontend_ = frontend;
  DCHECK(frontend);

  syncable::ModelTypeSet initial_types_with_nigori(initial_types);
  CHECK(sync_prefs_.get());
  if (sync_prefs_->HasSyncSetupCompleted()) {
    initial_types_with_nigori.Put(syncable::NIGORI);
  }

  registrar_.reset(new SyncBackendRegistrar(initial_types_with_nigori,
                                            name_,
                                            profile_,
                                            sync_thread_.message_loop()));
  initialization_state_ = CREATING_SYNC_MANAGER;
  InitCore(DoInitializeOptions(
      sync_thread_.message_loop(),
      registrar_.get(),
      &extensions_activity_monitor_,
      event_handler,
      sync_service_url,
      base::Bind(&MakeHttpBridgeFactory,
                 make_scoped_refptr(profile_->GetRequestContext())),
      credentials,
      &chrome_sync_notification_bridge_,
      &sync_notifier_factory_,
      delete_sync_data_folder,
      sync_prefs_->GetEncryptionBootstrapToken(),
      sync_api::SyncManager::NON_TEST,
      unrecoverable_error_handler,
      report_unrecoverable_error_function));
}
