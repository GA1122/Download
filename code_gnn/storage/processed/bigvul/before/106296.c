void SyncBackendHost::Core::CreateSyncNotifier(
    const scoped_refptr<net::URLRequestContextGetter>& request_context_getter) {
  const std::string& client_info = webkit_glue::GetUserAgent(GURL());
  SyncNotifierFactory sync_notifier_factory(client_info);
  sync_notifier_.reset(sync_notifier_factory.CreateSyncNotifier(
      *CommandLine::ForCurrentProcess(),
      request_context_getter));
}
