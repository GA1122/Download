void GpuCommandBufferStub::OnInitialize(
    IPC::Message* reply_message) {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnInitialize");
  DCHECK(!command_buffer_.get());

  command_buffer_.reset(new gpu::CommandBufferService(
      context_group_->transfer_buffer_manager()));

  if (!command_buffer_->Initialize()) {
    DLOG(ERROR) << "CommandBufferService failed to initialize.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  decoder_.reset(::gpu::gles2::GLES2Decoder::Create(context_group_.get()));

  scheduler_.reset(new gpu::GpuScheduler(command_buffer_.get(),
                                         decoder_.get(),
                                         decoder_.get()));
  if (preempt_by_counter_.get())
    scheduler_->SetPreemptByCounter(preempt_by_counter_);

  decoder_->set_engine(scheduler_.get());

  if (!handle_.is_null()) {
#if defined(OS_MACOSX) || defined(UI_COMPOSITOR_IMAGE_TRANSPORT)
    if (software_) {
      DLOG(ERROR) << "No software support.\n";
      OnInitializeFailed(reply_message);
      return;
    }
#endif

    surface_ = ImageTransportSurface::CreateSurface(
        channel_->gpu_channel_manager(),
        this,
        handle_);
  } else {
    GpuChannelManager* manager = channel_->gpu_channel_manager();
    surface_ = manager->GetDefaultOffscreenSurface();
  }

  if (!surface_.get()) {
    decoder_.reset();

    DLOG(ERROR) << "Failed to create surface.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  scoped_refptr<gfx::GLContext> context;
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableVirtualGLContexts) && channel_->share_group()) {
    context = channel_->share_group()->GetSharedContext();
    if (!context) {
      context = gfx::GLContext::CreateGLContext(
          channel_->share_group(),
          channel_->gpu_channel_manager()->GetDefaultOffscreenSurface(),
          gpu_preference_);
      channel_->share_group()->SetSharedContext(context);
    }
    DCHECK(context->GetHandle());
    context = new gpu::GLContextVirtual(channel_->share_group(),
                                        context,
                                        decoder_->AsWeakPtr());
    if (!context->Initialize(surface_, gpu_preference_)) {
      context = NULL;
      LOG(FATAL) << "Failed to initialize virtual GL context.";
    } else {
      LOG(INFO) << "Created virtual GL context.";
    }
  }
  if (!context) {
    context = gfx::GLContext::CreateGLContext(
        channel_->share_group(),
        surface_.get(),
        gpu_preference_);
  }
  if (!context) {
    decoder_.reset();

    DLOG(ERROR) << "Failed to create context.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  if (!context->MakeCurrent(surface_.get())) {
    decoder_.reset();
    LOG(ERROR) << "Failed to make context current.";
    OnInitializeFailed(reply_message);
    return;
  }

  if (!context->GetTotalGpuMemory(&total_gpu_memory_))
    total_gpu_memory_ = 0;

  if (!context_group_->has_program_cache()) {
    context_group_->set_program_cache(
        channel_->gpu_channel_manager()->program_cache());
  }

  if (!decoder_->Initialize(surface_,
                            context,
                            !surface_id(),
                            initial_size_,
                            disallowed_features_,
                            allowed_extensions_.c_str(),
                            requested_attribs_)) {
    DLOG(ERROR) << "Failed to initialize decoder.";
    OnInitializeFailed(reply_message);
    return;
  }

  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableGPUServiceLogging)) {
    decoder_->set_log_commands(true);
  }

  decoder_->SetMsgCallback(
      base::Bind(&GpuCommandBufferStub::SendConsoleMessage,
                 base::Unretained(this)));

  command_buffer_->SetPutOffsetChangeCallback(
      base::Bind(&GpuCommandBufferStub::PutChanged, base::Unretained(this)));
  command_buffer_->SetGetBufferChangeCallback(
      base::Bind(&gpu::GpuScheduler::SetGetBuffer,
                 base::Unretained(scheduler_.get())));
  command_buffer_->SetParseErrorCallback(
      base::Bind(&GpuCommandBufferStub::OnParseError, base::Unretained(this)));
  scheduler_->SetScheduledCallback(
      base::Bind(&GpuCommandBufferStub::OnReschedule, base::Unretained(this)));

  if (watchdog_) {
    scheduler_->SetCommandProcessedCallback(
        base::Bind(&GpuCommandBufferStub::OnCommandProcessed,
                   base::Unretained(this)));
  }

#if defined(OS_ANDROID)
  decoder_->SetStreamTextureManager(channel_->stream_texture_manager());
#endif

  if (parent_stub_for_initialization_) {
    decoder_->SetParent(parent_stub_for_initialization_->decoder_.get(),
                        parent_texture_for_initialization_);
    parent_stub_for_initialization_.reset();
    parent_texture_for_initialization_ = 0;
  }

  GpuCommandBufferMsg_Initialize::WriteReplyParams(reply_message, true);
  Send(reply_message);

  if (handle_.is_null() && !active_url_.is_empty()) {
    GpuChannelManager* gpu_channel_manager = channel_->gpu_channel_manager();
    gpu_channel_manager->Send(new GpuHostMsg_DidCreateOffscreenContext(
        active_url_));
  }
}
