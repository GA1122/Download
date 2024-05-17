void GpuCommandBufferStub::OnInitialize(
    IPC::Message* reply_message) {
  DCHECK(!command_buffer_.get());

  command_buffer_.reset(new gpu::CommandBufferService);

  if (!command_buffer_->Initialize()) {
    DLOG(ERROR) << "CommandBufferService failed to initialize.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  decoder_.reset(::gpu::gles2::GLES2Decoder::Create(context_group_.get()));

  scheduler_.reset(new gpu::GpuScheduler(command_buffer_.get(),
                                         decoder_.get(),
                                         decoder_.get()));

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
    surface_ = gfx::GLSurface::CreateOffscreenGLSurface(software_,
                                                        gfx::Size(1, 1));
  }

  if (!surface_.get()) {
    decoder_.reset();

    DLOG(ERROR) << "Failed to create surface.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  gfx::GpuPreference gpu_preference =
      channel_->ShouldPreferDiscreteGpu() ?
          gfx::PreferDiscreteGpu : gpu_preference_;

  context_ = gfx::GLContext::CreateGLContext(
      channel_->share_group(),
      surface_.get(),
      gpu_preference);
  if (!context_.get()) {
    decoder_.reset();

    DLOG(ERROR) << "Failed to create context.\n";
    OnInitializeFailed(reply_message);
    return;
  }

  if (!decoder_->Initialize(surface_.get(),
                            context_.get(),
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
      base::Bind(&gpu::GpuScheduler::PutChanged,
                 base::Unretained(scheduler_.get())));
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

  if (parent_stub_for_initialization_) {
    decoder_->SetParent(parent_stub_for_initialization_->decoder_.get(),
                        parent_texture_for_initialization_);
    parent_stub_for_initialization_.reset();
    parent_texture_for_initialization_ = 0;
  }

  GpuCommandBufferMsg_Initialize::WriteReplyParams(reply_message, true);
  Send(reply_message);

  channel_->gpu_channel_manager()->gpu_memory_manager()->ScheduleManage();
}
