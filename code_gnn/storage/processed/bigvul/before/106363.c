SyncBackendHost::SyncBackendHost(Profile* profile)
    : core_(new Core(ALLOW_THIS_IN_INITIALIZER_LIST(this))),
      core_thread_("Chrome_SyncCoreThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(profile),
      frontend_(NULL),
      sync_data_folder_path_(
          profile_->GetPath().Append(kSyncDataFolderName)),
      last_auth_error_(AuthError::None()),
      syncapi_initialized_(false) {
}
