URLRequestContext* BrowserContextIOData::CreateMainRequestContext(
    content::ProtocolHandlerMap& protocol_handlers,
    content::URLRequestInterceptorScopedVector request_interceptors) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  DCHECK(!main_request_context_);

  std::unique_ptr<net::CookieStore> cookie_store;
  scoped_refptr<net::SQLiteChannelIDStore> channel_id_store;
  if (!IsOffTheRecord() && !GetPath().empty()) {
    cookie_store =
        content::CreateCookieStore(
            content::CookieStoreConfig(GetPath().Append(kCookiesFilename),
                                       GetSessionCookieMode(),
                                       nullptr, nullptr));
    channel_id_store =
        new net::SQLiteChannelIDStore(
            GetPath().Append(kChannelIDFilename),
            content::BrowserThread::GetBlockingPool()->GetSequencedTaskRunner(
                base::SequencedWorkerPool::GetSequenceToken()));
  } else {
    cookie_store = content::CreateCookieStore(content::CookieStoreConfig());
  }

  cookie_store_owner_->set_store(std::move(cookie_store));

  IOThread::Globals* io_thread_globals = IOThread::instance()->globals();

  ssl_config_service_ = new SSLConfigService();
  http_user_agent_settings_.reset(new HttpUserAgentSettings(this));
  ftp_transaction_factory_.reset(
      new net::FtpNetworkLayer(io_thread_globals->host_resolver()));

  http_server_properties_.reset(new net::HttpServerPropertiesImpl());

  network_delegate_.reset(new NetworkDelegate(this));
  transport_security_state_.reset(new net::TransportSecurityState());
  transport_security_persister_.reset(
      new net::TransportSecurityPersister(
        transport_security_state_.get(),
        GetPath(),
        content::BrowserThread::GetTaskRunnerForThread(
          content::BrowserThread::FILE),
        IsOffTheRecord()));

  host_mapping_rules_.reset(new net::HostMappingRules());
  const std::vector<std::string>& host_mapping_rules =
      GetSharedData().host_mapping_rules;
  for (std::vector<std::string>::const_iterator it =
           host_mapping_rules.begin();
       it != host_mapping_rules.end(); ++it) {
    host_mapping_rules_->AddRuleFromString(*it);
  }

  main_request_context_.reset(new URLRequestContext());
  URLRequestContext* context = main_request_context_.get();
  net::URLRequestContextStorage* storage = context->storage();

  storage->set_ssl_config_service(ssl_config_service_.get());
  context->set_network_delegate(network_delegate_.get());
  context->set_http_user_agent_settings(http_user_agent_settings_.get());

  storage->set_channel_id_service(
      base::MakeUnique<net::ChannelIDService>(
          new net::DefaultChannelIDStore(channel_id_store.get()),
          base::WorkerPool::GetTaskRunner(true)));

  context->set_http_server_properties(http_server_properties_.get());

  context->set_cookie_store(cookie_store_owner_->store());

  context->set_transport_security_state(transport_security_state_.get());

  content::BrowserThread::PostTask(
      content::BrowserThread::FILE,
      FROM_HERE,
      base::Bind(&CleanupOldCacheDir, GetCachePath().Append(kCacheDirname)));

  std::unique_ptr<net::HttpCache::BackendFactory> cache_backend;
  if (IsOffTheRecord() || GetCachePath().empty()) {
    cache_backend = net::HttpCache::DefaultBackend::InMemory(0);
  } else {
    cache_backend.reset(new net::HttpCache::DefaultBackend(
          net::DISK_CACHE,
          net::CACHE_BACKEND_SIMPLE,
          GetCachePath().Append(kCacheDirname2),
          GetMaxCacheSizeHint() * 1024 * 1024,  
          content::BrowserThread::GetTaskRunnerForThread(
              content::BrowserThread::CACHE)));
  }

  net::HttpNetworkSession::Params session_params;
  session_params.host_resolver = context->host_resolver();
  session_params.cert_verifier = context->cert_verifier();
  session_params.channel_id_service = context->channel_id_service();
  session_params.transport_security_state =
      context->transport_security_state();
  session_params.cert_transparency_verifier =
      context->cert_transparency_verifier();
  session_params.ct_policy_enforcer = context->ct_policy_enforcer();
  session_params.proxy_service = context->proxy_service();
  session_params.ssl_config_service = context->ssl_config_service();
  session_params.http_auth_handler_factory =
      context->http_auth_handler_factory();
  session_params.http_server_properties =
      context->http_server_properties();
  session_params.net_log = context->net_log();
  session_params.host_mapping_rules = host_mapping_rules_.get();

  http_network_session_ =
      base::WrapUnique(new net::HttpNetworkSession(session_params));

  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    storage->set_http_transaction_factory(
        base::WrapUnique(
            new net::HttpCache(http_network_session_.get(),
                               std::move(cache_backend),
                               true)));
  }

  std::unique_ptr<net::URLRequestJobFactoryImpl> job_factory(
      new net::URLRequestJobFactoryImpl());

  bool set_protocol = false;
  for (content::ProtocolHandlerMap::iterator it = protocol_handlers.begin();
       it != protocol_handlers.end();
       ++it) {
    set_protocol =
        job_factory->SetProtocolHandler(it->first,
                                        base::WrapUnique(it->second.release()));
    DCHECK(set_protocol);
  }
  protocol_handlers.clear();

  set_protocol = job_factory->SetProtocolHandler(
      oxide::kFileScheme,
      base::WrapUnique(
          new net::FileProtocolHandler(
              content::BrowserThread::GetTaskRunnerForThread(
                  content::BrowserThread::FILE))));
  DCHECK(set_protocol);
  set_protocol = job_factory->SetProtocolHandler(
      oxide::kDataScheme,
      base::WrapUnique(new net::DataProtocolHandler()));
  DCHECK(set_protocol);

  set_protocol = job_factory->SetProtocolHandler(
      oxide::kFtpScheme,
      base::WrapUnique(new net::FtpProtocolHandler(
        ftp_transaction_factory_.get())));
  DCHECK(set_protocol);

  std::unique_ptr<net::URLRequestJobFactory> top_job_factory(
      new URLRequestDelegatedJobFactory(std::move(job_factory),
                                        this));

  for (content::URLRequestInterceptorScopedVector::reverse_iterator it =
          request_interceptors.rbegin();
       it != request_interceptors.rend();
       ++it) {
    top_job_factory.reset(
        new net::URLRequestInterceptingJobFactory(std::move(top_job_factory),
                                                  base::WrapUnique(*it)));
  }
  request_interceptors.weak_clear();

  storage->set_job_factory(std::move(top_job_factory));

  resource_context_->request_context_ = context;
  return main_request_context_.get();
}
