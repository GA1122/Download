bool GpuProcessHost::Init() {
  init_start_time_ = base::TimeTicks::Now();

  TRACE_EVENT_INSTANT0("gpu", "LaunchGpuProcess", TRACE_EVENT_SCOPE_THREAD);

  if (ServiceManagerConnection::GetForProcess()) {
    ServiceManagerConnection::GetForProcess()->AddConnectionFilter(
        std::make_unique<ConnectionFilterImpl>(process_->GetData().id));
  }

  process_->GetHost()->CreateChannelMojo();

  mode_ = GpuDataManagerImpl::GetInstance()->GetGpuMode();
  DCHECK_NE(mode_, gpu::GpuMode::DISABLED);

  if (in_process_) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(GetGpuMainThreadFactory());
    gpu::GpuPreferences gpu_preferences = GetGpuPreferencesFromCommandLine();
    GpuDataManagerImpl::GetInstance()->UpdateGpuPreferences(&gpu_preferences);
    in_process_gpu_thread_.reset(GetGpuMainThreadFactory()(
        InProcessChildThreadParams(
            base::ThreadTaskRunnerHandle::Get(),
            process_->GetInProcessMojoInvitation(),
            process_->child_connection()->service_token()),
        gpu_preferences));
    base::Thread::Options options;
#if defined(OS_WIN) || defined(OS_MACOSX)
    options.message_loop_type = base::MessageLoop::TYPE_UI;
#endif
#if defined(OS_ANDROID) || defined(OS_CHROMEOS)
    options.priority = base::ThreadPriority::DISPLAY;
#endif
    in_process_gpu_thread_->StartWithOptions(options);

    OnProcessLaunched();   
  } else if (!LaunchGpuProcess()) {
    return false;
  }

  process_->child_channel()
      ->GetAssociatedInterfaceSupport()
      ->GetRemoteAssociatedInterface(&gpu_main_ptr_);
  viz::mojom::GpuHostPtr host_proxy;
  gpu_host_binding_.Bind(mojo::MakeRequest(&host_proxy));

  discardable_memory::mojom::DiscardableSharedMemoryManagerPtr
      discardable_manager_ptr;
  auto discardable_request = mojo::MakeRequest(&discardable_manager_ptr);
  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                          base::BindOnce(&BindDiscardableMemoryRequestOnUI,
                                         std::move(discardable_request)));

  DCHECK(GetFontRenderParamsOnIO().initialized);
  gpu_main_ptr_->CreateGpuService(
      mojo::MakeRequest(&gpu_service_ptr_), std::move(host_proxy),
      std::move(discardable_manager_ptr), activity_flags_.CloneHandle(),
      GetFontRenderParamsOnIO().params.subpixel_rendering);

#if defined(USE_OZONE)
  InitOzone();
#endif   

  return true;
}
