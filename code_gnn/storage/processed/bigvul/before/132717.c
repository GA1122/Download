void ChromotingInstance::HandleConnect(const base::DictionaryValue& data) {
  std::string local_jid;
  std::string host_jid;
  std::string host_public_key;
  std::string auth_methods_str;
  std::string authentication_tag;
  std::vector<protocol::AuthenticationMethod> auth_methods;
  if (!data.GetString("hostJid", &host_jid) ||
      !data.GetString("hostPublicKey", &host_public_key) ||
      !data.GetString("localJid", &local_jid) ||
      !data.GetString("authenticationMethods", &auth_methods_str) ||
      !ParseAuthMethods(auth_methods_str, &auth_methods) ||
      !data.GetString("authenticationTag", &authentication_tag)) {
    LOG(ERROR) << "Invalid connect() data.";
    return;
  }

  std::string client_pairing_id;
  data.GetString("clientPairingId", &client_pairing_id);
  std::string client_paired_secret;
  data.GetString("clientPairedSecret", &client_paired_secret);

  protocol::FetchSecretCallback fetch_secret_callback;
  if (use_async_pin_dialog_) {
    fetch_secret_callback = base::Bind(
        &ChromotingInstance::FetchSecretFromDialog, weak_factory_.GetWeakPtr());
  } else {
    std::string shared_secret;
    if (!data.GetString("sharedSecret", &shared_secret)) {
      LOG(ERROR) << "sharedSecret not specified in connect().";
      return;
    }
    fetch_secret_callback =
        base::Bind(&ChromotingInstance::FetchSecretFromString, shared_secret);
  }

  std::string capabilities;
  if (data.HasKey("capabilities")) {
    if (!data.GetString("capabilities", &capabilities)) {
      LOG(ERROR) << "Invalid connect() data.";
      return;
    }
  }

  VLOG(0) << "Connecting to " << host_jid
          << ". Local jid: " << local_jid << ".";

#if defined(OS_NACL)
  std::string key_filter;
  if (!data.GetString("keyFilter", &key_filter)) {
    NOTREACHED();
    normalizing_input_filter_.reset(new protocol::InputFilter(&key_mapper_));
  } else if (key_filter == "mac") {
    normalizing_input_filter_.reset(
        new NormalizingInputFilterMac(&key_mapper_));
  } else if (key_filter == "cros") {
    normalizing_input_filter_.reset(
        new NormalizingInputFilterCros(&key_mapper_));
  } else {
    DCHECK(key_filter.empty());
    normalizing_input_filter_.reset(new protocol::InputFilter(&key_mapper_));
  }
#elif defined(OS_MACOSX)
  normalizing_input_filter_.reset(new NormalizingInputFilterMac(&key_mapper_));
#elif defined(OS_CHROMEOS)
  normalizing_input_filter_.reset(new NormalizingInputFilterCros(&key_mapper_));
#else
  normalizing_input_filter_.reset(new protocol::InputFilter(&key_mapper_));
#endif
  input_handler_.set_input_stub(normalizing_input_filter_.get());

  bool enable_video_decode_renderer = false;
  if (data.GetBoolean("enableVideoDecodeRenderer",
                      &enable_video_decode_renderer) &&
      enable_video_decode_renderer) {
    LogToWebapp("Initializing 3D renderer.");
    video_renderer_.reset(new PepperVideoRenderer3D());
    if (!video_renderer_->Initialize(this, context_, this))
      video_renderer_.reset();
  }

  if (!video_renderer_) {
    LogToWebapp("Initializing 2D renderer.");
    video_renderer_.reset(new PepperVideoRenderer2D());
    if (!video_renderer_->Initialize(this, context_, this))
      video_renderer_.reset();
  }

  CHECK(video_renderer_);

  if (!plugin_view_.is_null())
    video_renderer_->OnViewChanged(plugin_view_);

  scoped_ptr<AudioPlayer> audio_player(new PepperAudioPlayer(this));
  client_.reset(new ChromotingClient(&context_, this, video_renderer_.get(),
                                     audio_player.Pass()));

  mouse_input_filter_.set_input_stub(client_->input_stub());
  if (!plugin_view_.is_null()) {
    mouse_input_filter_.set_input_size(webrtc::DesktopSize(
        plugin_view_.GetRect().width(), plugin_view_.GetRect().height()));
  }

  signal_strategy_.reset(new DelegatingSignalStrategy(
      local_jid, base::Bind(&ChromotingInstance::SendOutgoingIq,
                            weak_factory_.GetWeakPtr())));

  scoped_ptr<protocol::TransportFactory> transport_factory(
      new protocol::LibjingleTransportFactory(
          signal_strategy_.get(),
          PepperPortAllocator::Create(this).Pass(),
          protocol::NetworkSettings(
              protocol::NetworkSettings::NAT_TRAVERSAL_FULL)));

  scoped_ptr<protocol::ThirdPartyClientAuthenticator::TokenFetcher>
      token_fetcher(new TokenFetcherProxy(
          base::Bind(&ChromotingInstance::FetchThirdPartyToken,
                     weak_factory_.GetWeakPtr()),
          host_public_key));
  scoped_ptr<protocol::Authenticator> authenticator(
      new protocol::NegotiatingClientAuthenticator(
          client_pairing_id, client_paired_secret, authentication_tag,
          fetch_secret_callback, token_fetcher.Pass(), auth_methods));

  client_->Start(signal_strategy_.get(), authenticator.Pass(),
                 transport_factory.Pass(), host_jid, capabilities);

  plugin_task_runner_->PostDelayedTask(
      FROM_HERE, base::Bind(&ChromotingInstance::SendPerfStats,
                            weak_factory_.GetWeakPtr()),
      base::TimeDelta::FromMilliseconds(kPerfStatsIntervalMs));
}
