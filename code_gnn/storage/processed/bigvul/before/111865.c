void ProfileSyncService::ConfigureDataTypeManager() {
  bool restart = false;
  if (!data_type_manager_.get()) {
    restart = true;
    data_type_manager_.reset(
        factory_->CreateDataTypeManager(backend_.get(),
                                        &data_type_controllers_));
    registrar_.Add(this,
                   chrome::NOTIFICATION_SYNC_CONFIGURE_START,
                   content::Source<DataTypeManager>(data_type_manager_.get()));
    registrar_.Add(this,
                   chrome::NOTIFICATION_SYNC_CONFIGURE_DONE,
                   content::Source<DataTypeManager>(data_type_manager_.get()));
    registrar_.Add(this,
                   chrome::NOTIFICATION_SYNC_CONFIGURE_BLOCKED,
                   content::Source<DataTypeManager>(data_type_manager_.get()));

    migrator_.reset(
        new browser_sync::BackendMigrator(
            profile_->GetDebugName(), GetUserShare(),
            this, data_type_manager_.get(),
            base::Bind(&ProfileSyncService::StartSyncingWithServer,
                       base::Unretained(this))));
  }

  const syncable::ModelTypeSet types = GetPreferredDataTypes();
  if (IsPassphraseRequiredForDecryption()) {
    DVLOG(1) << "ProfileSyncService::ConfigureDataTypeManager bailing out "
             << "because a passphrase required";
    NotifyObservers();
    return;
  }
  sync_api::ConfigureReason reason = sync_api::CONFIGURE_REASON_UNKNOWN;
  if (!HasSyncSetupCompleted()) {
    reason = sync_api::CONFIGURE_REASON_NEW_CLIENT;
  } else if (restart == false ||
             sync_api::InitialSyncEndedForTypes(types, GetUserShare())) {
    reason = sync_api::CONFIGURE_REASON_RECONFIGURATION;
  } else {
    DCHECK(restart);
    reason = sync_api::CONFIGURE_REASON_NEWLY_ENABLED_DATA_TYPE;
  }
  DCHECK(reason != sync_api::CONFIGURE_REASON_UNKNOWN);

  data_type_manager_->Configure(types, reason);
}
