void IOThread::InitSystemRequestContextOnIOThread() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!globals_->system_proxy_service.get());
  DCHECK(system_proxy_config_service_.get());

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  globals_->system_proxy_service.reset(
      ProxyServiceFactory::CreateProxyService(
          net_log_,
          globals_->proxy_script_fetcher_context.get(),
          globals_->system_network_delegate.get(),
          system_proxy_config_service_.release(),
          command_line));

  net::HttpNetworkSession::Params system_params;
  InitializeNetworkSessionParams(&system_params);
  system_params.net_log = net_log_;
  system_params.proxy_service = globals_->system_proxy_service.get();

  globals_->system_http_transaction_factory.reset(
      new net::HttpNetworkLayer(
          new net::HttpNetworkSession(system_params)));
  globals_->system_request_context.reset(
      ConstructSystemRequestContext(globals_, net_log_));

  sdch_manager_->set_sdch_fetcher(
      new SdchDictionaryFetcher(system_url_request_context_getter_.get()));
}
