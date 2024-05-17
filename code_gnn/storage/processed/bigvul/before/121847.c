void IOThread::CleanUp() {
  base::debug::LeakTracker<SafeBrowsingURLRequestContext>::CheckForLeaks();

  delete sdch_manager_;
  sdch_manager_ = NULL;

#if defined(USE_NSS) || defined(OS_IOS)
  net::ShutdownNSSHttpIO();
#endif

  system_url_request_context_getter_ = NULL;


  network_change_observer_.reset();

  system_proxy_config_service_.reset();

  delete globals_;
  globals_ = NULL;

  base::debug::LeakTracker<SystemURLRequestContextGetter>::CheckForLeaks();
}
