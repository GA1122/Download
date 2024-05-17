SyncBackendHost::SyncBackendHost(const std::string& name, Profile* profile)
    : core_(new Core(name, ALLOW_THIS_IN_INITIALIZER_LIST(this))),
      initialization_state_(NOT_INITIALIZED),
      sync_thread_("Chrome_SyncThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(profile),
      name_(name),
      sync_notifier_factory_(webkit_glue::GetUserAgent(GURL()),
                             profile_->GetRequestContext(),
                             *CommandLine::ForCurrentProcess()),
      frontend_(NULL),
      sync_data_folder_path_(
          profile_->GetPath().Append(kSyncDataFolderName)),
      last_auth_error_(AuthError::None()) {
}
