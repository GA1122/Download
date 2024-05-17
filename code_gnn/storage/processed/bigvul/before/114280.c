bool WebGraphicsContext3DCommandBufferImpl::MaybeInitializeGL(
    const char* allowed_extensions) {
  if (initialized_)
    return true;

  if (initialize_failed_)
    return false;

  TRACE_EVENT0("gpu", "WebGfxCtx3DCmdBfrImpl::MaybeInitializeGL");

  const char* preferred_extensions = "*";

  if (!CreateContext(surface_id_ != 0,
                     allowed_extensions ?
                         allowed_extensions : preferred_extensions)) {
    Destroy();
    return false;
  }

  if (gl_ && attributes_.noExtensions)
    gl_->EnableFeatureCHROMIUM("webgl_enable_glsl_webgl_validation");

  command_buffer_->SetChannelErrorCallback(
      base::Bind(&WebGraphicsContext3DCommandBufferImpl::OnContextLost,
                 weak_ptr_factory_.GetWeakPtr()));

  command_buffer_->SetOnConsoleMessageCallback(
      base::Bind(&WebGraphicsContext3DCommandBufferImpl::OnErrorMessage,
                 weak_ptr_factory_.GetWeakPtr()));

  client_error_message_callback_.reset(
      new WebGraphicsContext3DErrorMessageCallback(this));
  gl_->SetErrorMessageCallback(client_error_message_callback_.get());

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  free_command_buffer_when_invisible_ =
      command_line.HasSwitch(switches::kEnablePruneGpuCommandBuffers);

  {
    GLint alpha_bits = 0;
    getIntegerv(GL_ALPHA_BITS, &alpha_bits);
    attributes_.alpha = alpha_bits > 0;
    GLint depth_bits = 0;
    getIntegerv(GL_DEPTH_BITS, &depth_bits);
    attributes_.depth = depth_bits > 0;
    GLint stencil_bits = 0;
    getIntegerv(GL_STENCIL_BITS, &stencil_bits);
    attributes_.stencil = stencil_bits > 0;
    GLint samples = 0;
    getIntegerv(GL_SAMPLES, &samples);
    attributes_.antialias = samples > 0;
  }

  if (attributes_.shareResources) {
    base::AutoLock lock(g_all_shared_contexts_lock.Get());
    g_all_shared_contexts.Pointer()->insert(this);
  }

  command_buffer_->SetMemoryAllocationChangedCallback(base::Bind(
      &WebGraphicsContext3DCommandBufferImpl::OnMemoryAllocationChanged,
      weak_ptr_factory_.GetWeakPtr()));

  visible_ = true;
  initialized_ = true;
  return true;
}
