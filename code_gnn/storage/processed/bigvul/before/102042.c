void SyncBackendHost::Core::DoInitialize(const DoInitializeOptions& options) {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());

  if (options.delete_sync_data_folder) {
    DeleteSyncDataFolder();
  }

  bool success = file_util::CreateDirectory(host_->sync_data_folder_path());
  DCHECK(success);

  DCHECK(!registrar_);
  registrar_ = options.registrar;
  DCHECK(registrar_);

  sync_manager_.reset(new sync_api::SyncManager(name_));
  sync_manager_->AddObserver(this);
  const FilePath& path_str = host_->sync_data_folder_path();
  success = sync_manager_->Init(
      path_str,
      options.event_handler,
      options.service_url.host() + options.service_url.path(),
      options.service_url.EffectiveIntPort(),
      options.service_url.SchemeIsSecure(),
      host_->MakeHttpBridgeFactory(options.request_context_getter),
      options.registrar,
      MakeUserAgentForSyncApi(),
      options.credentials,
      host_->sync_notifier_factory_.CreateSyncNotifier(),
      options.restored_key_for_bootstrapping,
      options.setup_for_test_mode);
  DCHECK(success) << "Syncapi initialization failed!";
}
