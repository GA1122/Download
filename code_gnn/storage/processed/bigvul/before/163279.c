void BrowserMainLoop::PostMainMessageLoopStart() {
  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:SystemMonitor");
    system_monitor_.reset(new base::SystemMonitor);
  }
  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:PowerMonitor");
    std::unique_ptr<base::PowerMonitorSource> power_monitor_source(
        new base::PowerMonitorDeviceSource());
    power_monitor_.reset(
        new base::PowerMonitor(std::move(power_monitor_source)));
  }
  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:HighResTimerManager");
    hi_res_timer_manager_.reset(new base::HighResolutionTimerManager);
  }
  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:NetworkChangeNotifier");
    network_change_notifier_.reset(net::NetworkChangeNotifier::Create());
  }
  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:MediaFeatures");
    media::InitializeMediaLibrary();
  }
  {
    TRACE_EVENT0("startup",
                 "BrowserMainLoop::Subsystem:ContentWebUIController");
    WebUIControllerFactory::RegisterFactory(
        ContentWebUIControllerFactory::GetInstance());
  }

  {
    TRACE_EVENT0("startup", "BrowserMainLoop::Subsystem:OnlineStateObserver");
    online_state_observer_.reset(new BrowserOnlineStateObserver);
  }

  {
    system_stats_monitor_.reset(
        new base::trace_event::TraceEventSystemStatsMonitor(
            base::ThreadTaskRunnerHandle::Get()));
  }

  {
    base::SetRecordActionTaskRunner(
        BrowserThread::GetTaskRunnerForThread(BrowserThread::UI));
  }

  if (parameters_.create_discardable_memory) {
    discardable_shared_memory_manager_ =
        std::make_unique<discardable_memory::DiscardableSharedMemoryManager>();
    if (!parsed_command_line_.HasSwitch(switches::kSingleProcess)) {
      base::DiscardableMemoryAllocator::SetInstance(
          discardable_shared_memory_manager_.get());
    }
  }

  if (parts_)
    parts_->PostMainMessageLoopStart();

#if defined(OS_ANDROID)
  {
    TRACE_EVENT0("startup",
                 "BrowserMainLoop::Subsystem:BrowserMediaPlayerManager");
    if (UsingInProcessGpu()) {
      gpu::ScopedSurfaceRequestConduit::SetInstance(
          ScopedSurfaceRequestManager::GetInstance());
    }
  }

  if (!parsed_command_line_.HasSwitch(
      switches::kDisableScreenOrientationLock)) {
    TRACE_EVENT0("startup",
                 "BrowserMainLoop::Subsystem:ScreenOrientationProvider");
    screen_orientation_delegate_.reset(
        new ScreenOrientationDelegateAndroid());
  }
#endif

  if (parsed_command_line_.HasSwitch(
          switches::kEnableAggressiveDOMStorageFlushing)) {
    TRACE_EVENT0("startup",
                 "BrowserMainLoop::Subsystem:EnableAggressiveCommitDelay");
    DOMStorageArea::EnableAggressiveCommitDelay();
    LevelDBWrapperImpl::EnableAggressiveCommitDelay();
  }

  if (parsed_command_line_.HasSwitch(switches::kIsolateOrigins)) {
    ChildProcessSecurityPolicyImpl* policy =
        ChildProcessSecurityPolicyImpl::GetInstance();
    policy->AddIsolatedOriginsFromCommandLine(
        parsed_command_line_.GetSwitchValueASCII(switches::kIsolateOrigins));
  }

  InitSkiaEventTracer();
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      viz::ServerSharedBitmapManager::current(),
      "viz::ServerSharedBitmapManager", nullptr);
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      skia::SkiaMemoryDumpProvider::GetInstance(), "Skia", nullptr);
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      sql::SqlMemoryDumpProvider::GetInstance(), "Sql", nullptr);
}
