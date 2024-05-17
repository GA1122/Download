void RenderThreadImpl::InitializeWebKit(
    const scoped_refptr<base::SingleThreadTaskRunner>& resource_task_queue,
    blink::InterfaceRegistry* interface_registry) {
  DCHECK(!blink_platform_impl_);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

#ifdef ENABLE_VTUNE_JIT_INTERFACE
  if (command_line.HasSwitch(switches::kEnableVtune))
    gin::Debug::SetJitCodeEventHandler(vTune::GetVtuneCodeEventHandler());
#endif

  blink_platform_impl_.reset(
      new RendererBlinkPlatformImpl(renderer_scheduler_.get()));
  SetRuntimeFeaturesDefaultsAndUpdateFromArgs(command_line);
  GetContentClient()
      ->renderer()
      ->SetRuntimeFeaturesDefaultsBeforeBlinkInitialization();
  blink::Initialize(blink_platform_impl_.get(), interface_registry);

  v8::Isolate* isolate = blink::MainThreadIsolate();
  isolate->SetCreateHistogramFunction(CreateHistogram);
  isolate->SetAddHistogramSampleFunction(AddHistogramSample);
  renderer_scheduler_->SetRAILModeObserver(this);

  main_thread_compositor_task_runner_ =
      renderer_scheduler_->CompositorTaskRunner();

  main_input_callback_.Reset(
      base::Bind(base::IgnoreResult(&RenderThreadImpl::OnMessageReceived),
                 base::Unretained(this)));

  scoped_refptr<base::SingleThreadTaskRunner> resource_task_queue2;
  if (resource_task_queue) {
    resource_task_queue2 = resource_task_queue;
  } else {
    resource_task_queue2 = renderer_scheduler_->LoadingTaskRunner();
  }
  scoped_refptr<ResourceSchedulingFilter> filter(
      new ResourceSchedulingFilter(
          resource_task_queue2, resource_dispatcher_.get()));
  channel()->AddFilter(filter.get());
  resource_dispatcher_->SetResourceSchedulingFilter(filter);

  resource_message_filter_->SetMainThreadTaskRunner(
      resource_task_queue2);
  resource_dispatcher_->SetThreadTaskRunner(resource_task_queue2);

  if (!command_line.HasSwitch(switches::kDisableThreadedCompositing))
    InitializeCompositorThread();

  if (!input_event_filter_.get()) {
    input_event_filter_ = new MainThreadInputEventFilter(
        main_input_callback_.callback(), main_thread_compositor_task_runner_);
  }
  AddFilter(input_event_filter_.get());

  scoped_refptr<base::SingleThreadTaskRunner> compositor_impl_side_task_runner;
  if (compositor_task_runner_)
    compositor_impl_side_task_runner = compositor_task_runner_;
  else
    compositor_impl_side_task_runner = base::ThreadTaskRunnerHandle::Get();

  compositor_message_filter_ = new CompositorForwardingMessageFilter(
      compositor_impl_side_task_runner.get());
  AddFilter(compositor_message_filter_.get());

  RenderThreadImpl::RegisterSchemes();

  RenderMediaClient::Initialize();

  devtools_agent_message_filter_ = new DevToolsAgentFilter();
  AddFilter(devtools_agent_message_filter_.get());

  if (GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden()) {
    ScheduleIdleHandler(kLongIdleHandlerDelayMs);
  } else {
    isolate->IsolateInBackgroundNotification();
  }

  service_worker_message_filter_ =
      new ServiceWorkerMessageFilter(thread_safe_sender());
  AddFilter(service_worker_message_filter_->GetFilter());

  renderer_scheduler_->SetStoppingWhenBackgroundedEnabled(
      GetContentClient()->renderer()->AllowStoppingWhenProcessBackgrounded());

  SkGraphics::SetResourceCacheSingleAllocationByteLimit(
      kImageCacheSingleAllocationByteLimit);

  SkGraphics::SetImageGeneratorFromEncodedDataFactory(
      blink::WebImageGenerator::CreateAsSkImageGenerator);

  if (command_line.HasSwitch(switches::kExplicitlyAllowedPorts)) {
    std::string allowed_ports =
        command_line.GetSwitchValueASCII(switches::kExplicitlyAllowedPorts);
    net::SetExplicitlyAllowedPorts(allowed_ports);
  }
}
