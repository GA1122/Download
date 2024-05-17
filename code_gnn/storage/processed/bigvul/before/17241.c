void BrowserProcessMainImpl::Start(StartParams params) {
  CHECK_EQ(state_, STATE_NOT_STARTED) <<
      "Browser components cannot be started more than once";
  CHECK(params.delegate) << "No PlatformDelegate provided";

  platform_delegate_ = std::move(params.delegate);
  main_delegate_.reset(new ContentMainDelegate(platform_delegate_.get()));

  if (IsUnsupportedProcessModel(params.process_model)) {
    LOG(WARNING) <<
        "Using an unsupported process model. This may affect stability and "
        "security. Use at your own risk!";
  }
  process_model_ = params.process_model;

  state_ = STATE_STARTED;

#if defined(OS_POSIX)
  SetupAndVerifySignalHandlers();

  base::GlobalDescriptors::GetInstance()->Set(
      kPrimaryIPCChannel,
      kPrimaryIPCChannel + base::GlobalDescriptors::kBaseDescriptor);
#endif

  exit_manager_.reset(new base::AtExitManager());

  std::unique_ptr<base::Environment> env = base::Environment::Create();

  base::FilePath subprocess_exe = GetSubprocessPath(env.get());
  InitializeCommandLine(params.argv0,
                        subprocess_exe,
                        process_model_,
                        params.gl_implementation,
                        env.get());

  FormFactor form_factor =
      DetectFormFactorHint(params.primary_screen_size);
  base::CommandLine::ForCurrentProcess()
      ->AppendSwitchASCII(switches::kFormFactor,
                          GetFormFactorHintCommandLine(form_factor));
  AddFormFactorSpecificCommandLineArguments();

  PathService::Override(base::FILE_EXE, subprocess_exe);
  PathService::Override(base::FILE_MODULE, subprocess_exe);

  int exit_code;
  CHECK(!main_delegate_->BasicStartupComplete(&exit_code));

  content::InitializeMojo();

  content::ContentClientInitializer::Set(
      main_delegate_.get(),
      base::CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kSingleProcess));

#if defined(OS_LINUX)
  if (!params.nss_db_path.empty()) {
    PathService::OverrideAndCreateIfNeeded(crypto::DIR_NSSDB,
                                           params.nss_db_path,
                                           false, true);
  }
  crypto::EarlySetupForNSSInit();
#endif

  ui::RegisterPathProvider();
  content::RegisterPathProvider();
  content::RegisterContentSchemes(true);

  CHECK(base::i18n::InitializeICU()) << "Failed to initialize ICU";
#if defined(V8_USE_EXTERNAL_STARTUP_DATA)
  gin::V8Initializer::LoadV8Snapshot();
  gin::V8Initializer::LoadV8Natives();
#endif   

  main_delegate_->PreSandboxStartup();
  main_delegate_->SandboxInitialized(base::EmptyString());

  content::UtilityProcessHostImpl::RegisterUtilityMainThreadFactory(
      content::CreateInProcessUtilityThread);
  content::RenderProcessHostImpl::RegisterRendererMainThreadFactory(
      content::CreateInProcessRendererThread);
  content::GpuProcessHost::RegisterGpuMainThreadFactory(
      content::CreateInProcessGpuThread);

  browser_main_runner_.reset(content::BrowserMainRunner::Create());
  CHECK(browser_main_runner_.get()) << "Failed to create BrowserMainRunner";

  content::MainFunctionParams main_params(
      *base::CommandLine::ForCurrentProcess());
  CHECK_EQ(browser_main_runner_->Initialize(main_params), -1) <<
      "Failed to initialize BrowserMainRunner";
  CHECK_EQ(browser_main_runner_->Run(), 0) <<
      "Failed to run BrowserMainRunner";
}
