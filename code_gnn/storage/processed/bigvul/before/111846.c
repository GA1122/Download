SyncBackendHost::SyncBackendHost(const std::string& name,
                                 Profile* profile,
                                 const base::WeakPtr<SyncPrefs>& sync_prefs)
    : weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      sync_thread_("Chrome_SyncThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(profile),
      name_(name),
      core_(new Core(name, profile_->GetPath().Append(kSyncDataFolderName),
                     weak_ptr_factory_.GetWeakPtr())),
      initialization_state_(NOT_ATTEMPTED),
      sync_prefs_(sync_prefs),
      chrome_sync_notification_bridge_(profile_),
      sync_notifier_factory_(
          ParseNotifierOptions(*CommandLine::ForCurrentProcess(),
                               profile_->GetRequestContext()),
          content::GetUserAgent(GURL()),
          sync_prefs),
      frontend_(NULL) {
}
