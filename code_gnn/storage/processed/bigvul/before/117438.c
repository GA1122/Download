bool WebGraphicsContext3DCommandBufferImpl::MaybeInitializeGL() {
  if (context_)
    return true;
  if (initialize_failed_)
    return false;

  TRACE_EVENT0("gpu", "WebGfxCtx3DCmdBfrImpl::MaybeInitializeGL");

  const int alpha_size = attributes_.alpha ? 8 : 0;
  const int depth_size = attributes_.depth ? 24 : 0;
  const int stencil_size = attributes_.stencil ? 8 : 0;
  const int samples = attributes_.antialias ? 4 : 0;
  const int sample_buffers = attributes_.antialias ? 1 : 0;
  const int32 attribs[] = {
    ContentGLContext::ALPHA_SIZE, alpha_size,
    ContentGLContext::DEPTH_SIZE, depth_size,
    ContentGLContext::STENCIL_SIZE, stencil_size,
    ContentGLContext::SAMPLES, samples,
    ContentGLContext::SAMPLE_BUFFERS, sample_buffers,
    ContentGLContext::SHARE_RESOURCES, attributes_.shareResources ? 1 : 0,
    ContentGLContext::BIND_GENERATES_RESOURCES, 0,
    ContentGLContext::NONE,
  };

  const char* preferred_extensions = "*";

  {
    base::AutoLock lock(g_all_shared_contexts_lock.Get());
    ContentGLContext* share_group = NULL;
    if (attributes_.shareResources) {
      share_group = g_all_shared_contexts.Pointer()->empty() ?
          NULL : (*g_all_shared_contexts.Pointer()->begin())->context_;
    }

    if (surface_id_) {
      context_ = ContentGLContext::CreateViewContext(
          host_,
          surface_id_,
          share_group,
          preferred_extensions,
          attribs,
          active_url_,
          gpu_preference_);
    } else {
      context_ = ContentGLContext::CreateOffscreenContext(
          host_,
          gfx::Size(1, 1),
          share_group,
          preferred_extensions,
          attribs,
          active_url_,
          gpu_preference_);
    }
  }

  if (!context_)
    return false;

  gl_ = context_->GetImplementation();

  if (gl_ && attributes_.noExtensions)
    gl_->EnableFeatureCHROMIUM("webgl_enable_glsl_webgl_validation");

  context_->SetContextLostCallback(
      base::Bind(&WebGraphicsContext3DCommandBufferImpl::OnContextLost,
                 weak_ptr_factory_.GetWeakPtr()));

  context_->GetCommandBufferProxy()->SetOnConsoleMessageCallback(
      base::Bind(&WebGraphicsContext3DCommandBufferImpl::OnErrorMessage,
                 weak_ptr_factory_.GetWeakPtr()));

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDisableGLSLTranslator)) {
    context_->DisableShaderTranslation();
  }

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

  context_->SetMemoryAllocationChangedCallback(base::Bind(
      &WebGraphicsContext3DCommandBufferImpl::OnMemoryAllocationChanged,
      weak_ptr_factory_.GetWeakPtr()));

  return true;
}
