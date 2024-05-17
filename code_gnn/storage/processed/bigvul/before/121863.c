void IOThread::InitAsync() {
  TRACE_EVENT0("startup", "IOThread::InitAsync");
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

#if defined(USE_NSS) || defined(OS_IOS)
  net::SetMessageLoopForNSSHttpIO();
#endif

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();

  DCHECK(!globals_);
  globals_ = new Globals;

  network_change_observer_.reset(
      new LoggingNetworkChangeObserver(net_log_));

  net::NetworkChangeNotifier::InitHistogramWatcher();

  globals_->extension_event_router_forwarder =
      extension_event_router_forwarder_;
  ChromeNetworkDelegate* network_delegate =
      new ChromeNetworkDelegate(extension_event_router_forwarder_,
                                &system_enable_referrers_);
  if (command_line.HasSwitch(switches::kEnableClientHints))
    network_delegate->SetEnableClientHints();
  if (command_line.HasSwitch(switches::kDisableExtensionsHttpThrottling))
    network_delegate->NeverThrottleRequests();
  globals_->system_network_delegate.reset(network_delegate);
  globals_->host_resolver = CreateGlobalHostResolver(net_log_);
  UpdateDnsClientEnabled();
  globals_->cert_verifier.reset(net::CertVerifier::CreateDefault());
  globals_->transport_security_state.reset(new net::TransportSecurityState());
#if !defined(USE_OPENSSL)
  net::MultiLogCTVerifier* ct_verifier = new net::MultiLogCTVerifier();
  globals_->cert_transparency_verifier.reset(ct_verifier);

  ct_verifier->AddLog(net::ct::CreateGooglePilotLogVerifier().Pass());
  ct_verifier->AddLog(net::ct::CreateGoogleAviatorLogVerifier().Pass());
  ct_verifier->AddLog(net::ct::CreateGoogleRocketeerLogVerifier().Pass());

  if (command_line.HasSwitch(switches::kCertificateTransparencyLog)) {
    std::string switch_value = command_line.GetSwitchValueASCII(
        switches::kCertificateTransparencyLog);
    size_t delim_pos = switch_value.find(":");
    CHECK(delim_pos != std::string::npos)
        << "CT log description not provided (switch format"
           " is 'description:base64_key')";
    std::string log_description(switch_value.substr(0, delim_pos));
    std::string ct_public_key_data;
    CHECK(base::Base64Decode(
          switch_value.substr(delim_pos + 1),
          &ct_public_key_data)) << "Unable to decode CT public key.";
    scoped_ptr<net::CTLogVerifier> external_log_verifier(
        net::CTLogVerifier::Create(ct_public_key_data, log_description));
    CHECK(external_log_verifier) << "Unable to parse CT public key.";
    ct_verifier->AddLog(external_log_verifier.Pass());
  }
#else
  if (command_line.HasSwitch(switches::kCertificateTransparencyLog)) {
    LOG(DFATAL) << "Certificate Transparency is not yet supported in Chrome "
                   "builds using OpenSSL.";
  }
#endif
  globals_->ssl_config_service = GetSSLConfigService();
#if defined(OS_ANDROID) || defined(OS_IOS)
  if (DataReductionProxySettings::IsDataReductionProxyAllowed()) {
    spdyproxy_auth_origins_ =
        DataReductionProxySettings::GetDataReductionProxies();
  }
#endif   
  globals_->http_auth_handler_factory.reset(CreateDefaultAuthHandlerFactory(
      globals_->host_resolver.get()));
  globals_->http_server_properties.reset(new net::HttpServerPropertiesImpl());
  globals_->proxy_script_fetcher_proxy_service.reset(
      net::ProxyService::CreateDirectWithNetLog(net_log_));
  globals_->system_cookie_store = new net::CookieMonster(NULL, NULL);
  globals_->system_server_bound_cert_service.reset(
      new net::ServerBoundCertService(
          new net::DefaultServerBoundCertStore(NULL),
          base::WorkerPool::GetTaskRunner(true)));
  globals_->dns_probe_service.reset(new chrome_browser_net::DnsProbeService());
  globals_->load_time_stats.reset(new chrome_browser_net::LoadTimeStats());
  globals_->host_mapping_rules.reset(new net::HostMappingRules());
  globals_->http_user_agent_settings.reset(
      new BasicHttpUserAgentSettings(std::string()));
  if (command_line.HasSwitch(switches::kHostRules)) {
    TRACE_EVENT_BEGIN0("startup", "IOThread::InitAsync:SetRulesFromString");
    globals_->host_mapping_rules->SetRulesFromString(
        command_line.GetSwitchValueASCII(switches::kHostRules));
    TRACE_EVENT_END0("startup", "IOThread::InitAsync:SetRulesFromString");
  }
  if (command_line.HasSwitch(switches::kIgnoreCertificateErrors))
    globals_->ignore_certificate_errors = true;
  if (command_line.HasSwitch(switches::kTestingFixedHttpPort)) {
    globals_->testing_fixed_http_port =
        GetSwitchValueAsInt(command_line, switches::kTestingFixedHttpPort);
  }
  if (command_line.HasSwitch(switches::kTestingFixedHttpsPort)) {
    globals_->testing_fixed_https_port =
        GetSwitchValueAsInt(command_line, switches::kTestingFixedHttpsPort);
  }
  ConfigureQuic(command_line);
  if (command_line.HasSwitch(
          switches::kEnableUserAlternateProtocolPorts)) {
    globals_->enable_user_alternate_protocol_ports = true;
  }
  InitializeNetworkOptions(command_line);

  net::HttpNetworkSession::Params session_params;
  InitializeNetworkSessionParams(&session_params);
  session_params.net_log = net_log_;
  session_params.proxy_service =
      globals_->proxy_script_fetcher_proxy_service.get();

  TRACE_EVENT_BEGIN0("startup", "IOThread::InitAsync:HttpNetworkSession");
  scoped_refptr<net::HttpNetworkSession> network_session(
      new net::HttpNetworkSession(session_params));
  globals_->proxy_script_fetcher_http_transaction_factory
      .reset(new net::HttpNetworkLayer(network_session.get()));
  TRACE_EVENT_END0("startup", "IOThread::InitAsync:HttpNetworkSession");
  scoped_ptr<net::URLRequestJobFactoryImpl> job_factory(
      new net::URLRequestJobFactoryImpl());
  job_factory->SetProtocolHandler(chrome::kDataScheme,
                                  new net::DataProtocolHandler());
  job_factory->SetProtocolHandler(
      chrome::kFileScheme,
      new net::FileProtocolHandler(
          content::BrowserThread::GetBlockingPool()->
              GetTaskRunnerWithShutdownBehavior(
                  base::SequencedWorkerPool::SKIP_ON_SHUTDOWN)));
#if !defined(DISABLE_FTP_SUPPORT)
  globals_->proxy_script_fetcher_ftp_transaction_factory.reset(
      new net::FtpNetworkLayer(globals_->host_resolver.get()));
  job_factory->SetProtocolHandler(
      content::kFtpScheme,
      new net::FtpProtocolHandler(
          globals_->proxy_script_fetcher_ftp_transaction_factory.get()));
#endif
  globals_->proxy_script_fetcher_url_request_job_factory =
      job_factory.PassAs<net::URLRequestJobFactory>();

  globals_->throttler_manager.reset(new net::URLRequestThrottlerManager());
  globals_->throttler_manager->set_net_log(net_log_);
  globals_->throttler_manager->set_enable_thread_checks(true);

  globals_->proxy_script_fetcher_context.reset(
      ConstructProxyScriptFetcherContext(globals_, net_log_));

  globals_->network_time_notifier.reset(
      new net::NetworkTimeNotifier(
          scoped_ptr<base::TickClock>(new base::DefaultTickClock())));

  sdch_manager_ = new net::SdchManager();

#if defined(OS_MACOSX) && !defined(OS_IOS)
  BrowserThread::PostTask(BrowserThread::UI,
                          FROM_HERE,
                          base::Bind(&ObserveKeychainEvents));
#endif

  BrowserThread::PostTask(BrowserThread::UI,
                          FROM_HERE,
                          base::Bind(&IOThread::InitSystemRequestContext,
                                     weak_factory_.GetWeakPtr()));
}
