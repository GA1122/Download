void ChromeContentUtilityClient::UtilityThreadStarted() {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  extensions::utility_handler::UtilityThreadStarted();
#endif

  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kUtilityProcessRunningElevated))
    utility_process_running_elevated_ = true;

  content::ServiceManagerConnection* connection =
      content::ChildThread::Get()->GetServiceManagerConnection();

  if (!connection)
    return;

  auto registry = base::MakeUnique<service_manager::BinderRegistry>();
#if BUILDFLAG(ENABLE_EXTENSIONS)
  extensions::ExposeInterfacesToBrowser(registry.get(),
                                        utility_process_running_elevated_);
  extensions::utility_handler::ExposeInterfacesToBrowser(
      registry.get(), utility_process_running_elevated_);
#endif
  if (!utility_process_running_elevated_) {
    registry->AddInterface(base::Bind(&FilePatcherImpl::Create),
                           base::ThreadTaskRunnerHandle::Get());
#if !defined(OS_ANDROID)
    registry->AddInterface(base::Bind(CreateResourceUsageReporter),
                           base::ThreadTaskRunnerHandle::Get());
    registry->AddInterface(
        base::Bind(&media_router::DialDeviceDescriptionParserImpl::Create),
        base::ThreadTaskRunnerHandle::Get());
#endif   
#if defined(OS_WIN)
    registry->AddInterface(base::Bind(&ShellHandlerImpl::Create),
                           base::ThreadTaskRunnerHandle::Get());
#endif
#if defined(OS_CHROMEOS)
    registry->AddInterface(base::Bind(&ZipFileCreatorImpl::Create),
                           base::ThreadTaskRunnerHandle::Get());
#endif
#if defined(FULL_SAFE_BROWSING)
    registry->AddInterface(base::Bind(&SafeArchiveAnalyzerImpl::Create),
                           base::ThreadTaskRunnerHandle::Get());
#endif
  }

  connection->AddConnectionFilter(
      base::MakeUnique<content::SimpleConnectionFilter>(std::move(registry)));
}
