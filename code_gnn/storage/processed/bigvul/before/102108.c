SyncBackendHost::SyncBackendHost()
    : initialization_state_(NOT_INITIALIZED),
      sync_thread_("Chrome_SyncThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(NULL),
      name_("Unknown"),
      sync_notifier_factory_(webkit_glue::GetUserAgent(GURL()),
                             NULL,
                             *CommandLine::ForCurrentProcess()),
      frontend_(NULL),
      last_auth_error_(AuthError::None()) {
}
