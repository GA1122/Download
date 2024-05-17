void SyncBackendHost::Core::DoInitialize(const DoInitializeOptions& options) {
  DCHECK(MessageLoop::current() == host_->core_thread_.message_loop());
  processing_passphrase_ = false;

  if (options.delete_sync_data_folder) {
    DeleteSyncDataFolder();
  }

  bool success = file_util::CreateDirectory(host_->sync_data_folder_path());
  DCHECK(success);

  syncapi_->AddObserver(this);
  const FilePath& path_str = host_->sync_data_folder_path();
  success = syncapi_->Init(
      path_str,
      (options.service_url.host() + options.service_url.path()).c_str(),
      options.service_url.EffectiveIntPort(),
      options.service_url.SchemeIsSecure(),
      options.http_bridge_factory,
      host_,   
      MakeUserAgentForSyncapi().c_str(),
      options.credentials,
      sync_notifier_.get(),
      options.restored_key_for_bootstrapping,
      options.setup_for_test_mode);
  DCHECK(success) << "Syncapi initialization failed!";
}
