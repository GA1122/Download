SyncBackendHost::SyncBackendHost()
    : core_thread_("Chrome_SyncCoreThread"),
      frontend_loop_(MessageLoop::current()),
      profile_(NULL),
      frontend_(NULL),
      last_auth_error_(AuthError::None()),
      syncapi_initialized_(false) {
}
