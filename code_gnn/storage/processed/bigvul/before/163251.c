int BrowserMainLoop::BrowserThreadsStarted() {
  TRACE_EVENT0("startup", "BrowserMainLoop::BrowserThreadsStarted");

  InitializeMojo();

  const bool is_mus = IsUsingMus();
#if defined(USE_AURA)
  if (is_mus) {
    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kIsRunningInMash);
    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kEnableSurfaceSynchronization);
  }
#endif

  HistogramSynchronizer::GetInstance();
#if defined(OS_ANDROID) || defined(OS_CHROMEOS)
  base::PlatformThread::SetCurrentThreadPriority(base::ThreadPriority::DISPLAY);
#endif

#if BUILDFLAG(ENABLE_VULKAN)
  if (parsed_command_line_.HasSwitch(switches::kEnableVulkan))
    gpu::InitializeVulkan();
#endif

  InitShaderCacheFactorySingleton(
      BrowserThread::GetTaskRunnerForThread(BrowserThread::IO));

  bool always_uses_gpu = true;
  bool established_gpu_channel = false;
#if defined(OS_ANDROID)
  established_gpu_channel = false;
  always_uses_gpu = ShouldStartGpuProcessOnBrowserStartup();
  BrowserGpuChannelHostFactory::Initialize(established_gpu_channel);
#elif defined(USE_AURA) || defined(OS_MACOSX)
  established_gpu_channel = true;
  if (parsed_command_line_.HasSwitch(switches::kDisableGpu) ||
      parsed_command_line_.HasSwitch(switches::kDisableGpuCompositing) ||
      parsed_command_line_.HasSwitch(switches::kDisableGpuEarlyInit) ||
      is_mus) {
    established_gpu_channel = always_uses_gpu = false;
  }

  if (!is_mus) {
    host_frame_sink_manager_ = std::make_unique<viz::HostFrameSinkManager>();

    BrowserGpuChannelHostFactory::Initialize(established_gpu_channel);

    if (parsed_command_line_.HasSwitch(switches::kEnableViz)) {
      auto transport_factory = std::make_unique<VizProcessTransportFactory>(
          BrowserGpuChannelHostFactory::instance(), GetResizeTaskRunner());
      transport_factory->ConnectHostFrameSinkManager();
      ImageTransportFactory::SetFactory(std::move(transport_factory));
    } else {
      auto surface_lifetime_type =
          parsed_command_line_.HasSwitch(switches::kDisableSurfaceReferences)
              ? viz::SurfaceManager::LifetimeType::SEQUENCES
              : viz::SurfaceManager::LifetimeType::REFERENCES;
      frame_sink_manager_impl_ =
          std::make_unique<viz::FrameSinkManagerImpl>(surface_lifetime_type);

      surface_utils::ConnectWithLocalFrameSinkManager(
          host_frame_sink_manager_.get(), frame_sink_manager_impl_.get());

      compositing_mode_reporter_impl_ =
          std::make_unique<viz::CompositingModeReporterImpl>();

      ImageTransportFactory::SetFactory(
          std::make_unique<GpuProcessTransportFactory>(
              BrowserGpuChannelHostFactory::instance(),
              compositing_mode_reporter_impl_.get(), GetResizeTaskRunner()));
    }
  }

#if defined(USE_AURA)
  if (env_->mode() == aura::Env::Mode::LOCAL) {
    env_->set_context_factory(GetContextFactory());
    env_->set_context_factory_private(GetContextFactoryPrivate());
  }
#endif   
#endif   

#if defined(OS_ANDROID)
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      tracing::GraphicsMemoryDumpProvider::GetInstance(), "AndroidGraphics",
      nullptr);
#endif

  {
    TRACE_EVENT0("startup", "BrowserThreadsStarted::Subsystem:AudioMan");
    CreateAudioManager();
  }

  {
    TRACE_EVENT0("startup", "BrowserThreadsStarted::Subsystem:MidiService");
    midi_service_.reset(new midi::MidiService);
  }

#if defined(OS_WIN)
  if (base::FeatureList::IsEnabled(features::kHighDynamicRange))
    HDRProxy::Initialize();
  system_message_window_.reset(new media::SystemMessageWindowWin);
#elif defined(OS_LINUX) && defined(USE_UDEV)
  device_monitor_linux_.reset(
      new media::DeviceMonitorLinux(io_thread_->task_runner()));
#elif defined(OS_MACOSX)
  device_monitor_mac_.reset(
      new media::DeviceMonitorMac(audio_manager_->GetTaskRunner()));
#endif

  {
    TRACE_EVENT0("startup",
      "BrowserMainLoop::BrowserThreadsStarted:InitResourceDispatcherHost");
    resource_dispatcher_host_.reset(new ResourceDispatcherHostImpl(
        base::Bind(&DownloadResourceHandler::Create),
        BrowserThread::GetTaskRunnerForThread(BrowserThread::IO),
        !parsed_command_line_.HasSwitch(switches::kDisableResourceScheduler)));
    GetContentClient()->browser()->ResourceDispatcherHostCreated();

    loader_delegate_.reset(new LoaderDelegateImpl());
    resource_dispatcher_host_->SetLoaderDelegate(loader_delegate_.get());
  }

  {
    TRACE_EVENT0("startup",
      "BrowserMainLoop::BrowserThreadsStarted:InitMediaStreamManager");
    media_stream_manager_.reset(new MediaStreamManager(
        audio_system_.get(), audio_manager_->GetTaskRunner()));
  }

  {
    TRACE_EVENT0("startup",
      "BrowserMainLoop::BrowserThreadsStarted:InitSpeechRecognition");
    speech_recognition_manager_.reset(new SpeechRecognitionManagerImpl(
        audio_system_.get(), audio_manager_.get(),
        media_stream_manager_.get()));
  }

  {
    TRACE_EVENT0(
        "startup",
        "BrowserMainLoop::BrowserThreadsStarted::InitUserInputMonitor");
    user_input_monitor_ = media::UserInputMonitor::Create(
        io_thread_->task_runner(), main_thread_->task_runner());
  }

  {
    TRACE_EVENT0("startup",
      "BrowserMainLoop::BrowserThreadsStarted::SaveFileManager");
    save_file_manager_ = new SaveFileManager();
  }

  std::vector<base::PlatformThreadId> allowed_clipboard_threads;
  allowed_clipboard_threads.push_back(base::PlatformThread::CurrentId());
#if defined(OS_WIN)
  allowed_clipboard_threads.push_back(io_thread_->GetThreadId());
#endif
  ui::Clipboard::SetAllowedThreads(allowed_clipboard_threads);

  if (GpuDataManagerImpl::GetInstance()->GpuAccessAllowed(nullptr) &&
      !established_gpu_channel && always_uses_gpu && !UsingInProcessGpu() &&
      !is_mus) {
    TRACE_EVENT_INSTANT0("gpu", "Post task to launch GPU process",
                         TRACE_EVENT_SCOPE_THREAD);
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(base::IgnoreResult(&GpuProcessHost::Get),
                       GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED,
                       true  ));
  }

#if defined(OS_MACOSX)
  ThemeHelperMac::GetInstance();
  SystemHotkeyHelperMac::GetInstance()->DeferredLoadSystemHotkeys();
#endif   

#if defined(OS_ANDROID)
  media::SetMediaDrmBridgeClient(GetContentClient()->GetMediaDrmBridgeClient());
#endif

  return result_code_;
}
