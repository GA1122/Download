SyncBackendHost::SyncBackendHost(Profile* profile)
    : weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      sync_thread_("Chrome_SyncThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(profile),
      name_("Unknown"),
      initialization_state_(NOT_ATTEMPTED),
      chrome_sync_notification_bridge_(profile_),
      sync_notifier_factory_(
          ParseNotifierOptions(*CommandLine::ForCurrentProcess(),
                               profile_->GetRequestContext()),
          content::GetUserAgent(GURL()),
          base::WeakPtr<sync_notifier::InvalidationStateTracker>()),
      frontend_(NULL) {
}
