bool LiveSyncTest::IsTestServerRunning() {
  CommandLine* cl = CommandLine::ForCurrentProcess();
  std::string sync_url = cl->GetSwitchValueASCII(switches::kSyncServiceURL);
  GURL sync_url_status(sync_url.append("/healthz"));
  SyncServerStatusChecker delegate;
  URLFetcher fetcher(sync_url_status, URLFetcher::GET, &delegate);
  fetcher.set_request_context(Profile::GetDefaultRequestContext());
  fetcher.Start();
  ui_test_utils::RunMessageLoop();
  return delegate.running();
}
