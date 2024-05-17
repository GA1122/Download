void ChromeContentBrowserClient::RenderProcessWillLaunch(
    content::RenderProcessHost* host,
    service_manager::mojom::ServiceRequest* service_request) {
  int id = host->GetID();
  Profile* profile = Profile::FromBrowserContext(host->GetBrowserContext());
  host->AddFilter(new ChromeRenderMessageFilter(id, profile));
#if BUILDFLAG(ENABLE_EXTENSIONS)
  host->AddFilter(new cast::CastTransportHostFilter(profile));
#endif
#if BUILDFLAG(ENABLE_PRINTING)
  host->AddFilter(new printing::PrintingMessageFilter(id, profile));
#endif
  host->AddFilter(new prerender::PrerenderMessageFilter(id, profile));
  host->AddFilter(new TtsMessageFilter(host->GetBrowserContext()));
  WebRtcLoggingHandlerHost* webrtc_logging_handler_host =
      new WebRtcLoggingHandlerHost(id, profile,
                                   g_browser_process->webrtc_log_uploader());
  host->AddFilter(webrtc_logging_handler_host);
  host->SetUserData(
      WebRtcLoggingHandlerHost::kWebRtcLoggingHandlerHostKey,
      std::make_unique<base::UserDataAdapter<WebRtcLoggingHandlerHost>>(
          webrtc_logging_handler_host));

  AudioDebugRecordingsHandler* audio_debug_recordings_handler =
      new AudioDebugRecordingsHandler(profile);
  host->SetUserData(
      AudioDebugRecordingsHandler::kAudioDebugRecordingsHandlerKey,
      std::make_unique<base::UserDataAdapter<AudioDebugRecordingsHandler>>(
          audio_debug_recordings_handler));

#if BUILDFLAG(ENABLE_NACL)
  host->AddFilter(new nacl::NaClHostMessageFilter(id, profile->IsOffTheRecord(),
                                                  profile->GetPath()));
#endif

#if defined(OS_ANDROID)
  host->AddFilter(
      new cdm::CdmMessageFilterAndroid(!profile->IsOffTheRecord(), false));

  host->SetUserData(
      CrashMemoryMetricsCollector::kCrashMemoryMetricsCollectorKey,
      std::make_unique<CrashMemoryMetricsCollector>(host));
#endif

  Profile* original_profile = profile->GetOriginalProfile();
  RendererUpdaterFactory::GetForProfile(original_profile)
      ->InitializeRenderer(host);

  for (size_t i = 0; i < extra_parts_.size(); ++i)
    extra_parts_[i]->RenderProcessWillLaunch(host);

  service_manager::mojom::ServicePtr service;
  *service_request = mojo::MakeRequest(&service);
  service_manager::mojom::PIDReceiverPtr pid_receiver;
  service_manager::Identity renderer_identity = host->GetChildIdentity();
  ChromeService::GetInstance()->connector()->RegisterServiceInstance(
      service_manager::Identity(chrome::mojom::kRendererServiceName,
                                renderer_identity.instance_group(),
                                renderer_identity.instance_id(),
                                base::Token::CreateRandom()),
      std::move(service), mojo::MakeRequest(&pid_receiver));
}
