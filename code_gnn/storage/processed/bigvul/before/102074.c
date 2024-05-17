void SyncBackendHost::Initialize(
    SyncFrontend* frontend,
    const WeakHandle<JsEventHandler>& event_handler,
    const GURL& sync_service_url,
    const syncable::ModelTypeSet& initial_types,
    const SyncCredentials& credentials,
    bool delete_sync_data_folder) {
  if (!sync_thread_.Start())
    return;

  frontend_ = frontend;
  DCHECK(frontend);

  syncable::ModelTypeSet initial_types_with_nigori(initial_types);
  if (profile_->GetPrefs()->GetBoolean(prefs::kSyncHasSetupCompleted))
    initial_types_with_nigori.insert(syncable::NIGORI);

  registrar_.reset(new SyncBackendRegistrar(initial_types_with_nigori,
                                            name_,
                                            profile_,
                                            sync_thread_.message_loop()));

  InitCore(Core::DoInitializeOptions(
      registrar_.get(),
      event_handler,
      sync_service_url,
      profile_->GetRequestContext(),
      credentials,
      delete_sync_data_folder,
      RestoreEncryptionBootstrapToken(),
      false));
}
