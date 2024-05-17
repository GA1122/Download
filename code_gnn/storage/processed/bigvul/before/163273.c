void BrowserMainLoop::InitializeMojo() {
  if (!parsed_command_line_.HasSwitch(switches::kSingleProcess)) {
    bool sync_call_allowed = false;
    MojoResult result = mojo::edk::SetProperty(
        MOJO_PROPERTY_TYPE_SYNC_CALL_ALLOWED, &sync_call_allowed);
    DCHECK_EQ(MOJO_RESULT_OK, result);
  }

  mojo_ipc_support_.reset(new mojo::edk::ScopedIPCSupport(
      BrowserThread::GetTaskRunnerForThread(BrowserThread::IO),
      mojo::edk::ScopedIPCSupport::ShutdownPolicy::FAST));

  service_manager_context_.reset(new ServiceManagerContext);
#if defined(OS_MACOSX)
  mojo::edk::SetMachPortProvider(MachBroker::GetInstance());
#endif   
  GetContentClient()->OnServiceManagerConnected(
      ServiceManagerConnection::GetForProcess());

  tracing_controller_ = std::make_unique<content::TracingControllerImpl>();
  content::BackgroundTracingManagerImpl::GetInstance()
      ->AddMetadataGeneratorFunction();

  service_manager::Connector* connector =
      content::ServiceManagerConnection::GetForProcess()->GetConnector();
  memory_instrumentation::ClientProcessImpl::Config config(
      connector, resource_coordinator::mojom::kServiceName,
      memory_instrumentation::mojom::ProcessType::BROWSER);
  memory_instrumentation::ClientProcessImpl::CreateInstance(config);

  if (parsed_command_line_.HasSwitch(switches::kTraceStartup)) {
    base::trace_event::TraceConfig trace_config(
        parsed_command_line_.GetSwitchValueASCII(switches::kTraceStartup),
        base::trace_event::RECORD_UNTIL_FULL);
    TracingController::GetInstance()->StartTracing(
        trace_config, TracingController::StartTracingDoneCallback());
  } else if (parsed_command_line_.HasSwitch(switches::kTraceToConsole)) {
    TracingController::GetInstance()->StartTracing(
        tracing::GetConfigForTraceToConsole(),
        TracingController::StartTracingDoneCallback());
  } else if (tracing::TraceConfigFile::GetInstance()->IsEnabled()) {
    TracingController::GetInstance()->StartTracing(
        tracing::TraceConfigFile::GetInstance()->GetTraceConfig(),
        TracingController::StartTracingDoneCallback());
  }
  if (is_tracing_startup_for_duration_) {
    TRACE_EVENT0("startup", "BrowserMainLoop::InitStartupTracingForDuration");
    InitStartupTracingForDuration(parsed_command_line_);
  }

  if (parts_) {
    parts_->ServiceManagerConnectionStarted(
        ServiceManagerConnection::GetForProcess());
  }
}
