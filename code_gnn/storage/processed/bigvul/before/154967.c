void WebGLRenderingContextBase::MaybeRestoreContext(TimerBase*) {
  DCHECK(isContextLost());

  if (!restore_allowed_)
    return;

  if (canvas()) {
    LocalFrame* frame = canvas()->GetDocument().GetFrame();
    if (!frame)
      return;
    if (frame->Client()->ShouldBlockWebGL())
      return;

    Settings* settings = frame->GetSettings();
    if (settings && ((context_type_ == Platform::kWebGL1ContextType &&
                      !settings->GetWebGL1Enabled()) ||
                     ((context_type_ == Platform::kWebGL2ContextType ||
                       context_type_ == Platform::kWebGL2ComputeContextType) &&
                      !settings->GetWebGL2Enabled()))) {
      return;
    }
  }

  DCHECK(!GetDrawingBuffer());

  auto* execution_context = Host()->GetTopExecutionContext();
  Platform::ContextAttributes attributes = ToPlatformContextAttributes(
      CreationAttributes(), context_type_,
      SupportOwnOffscreenSurface(execution_context));
  Platform::GraphicsInfo gl_info;
  std::unique_ptr<WebGraphicsContext3DProvider> context_provider;
  bool using_gpu_compositing;
  const auto& url = Host()->GetExecutionContextUrl();

  if (IsMainThread()) {
    using_gpu_compositing = !Platform::Current()->IsGpuCompositingDisabled();
    context_provider =
        Platform::Current()->CreateOffscreenGraphicsContext3DProvider(
            attributes, url, &gl_info);
  } else {
    context_provider = CreateContextProviderOnWorkerThread(
        attributes, &gl_info, &using_gpu_compositing, url);
  }
  scoped_refptr<DrawingBuffer> buffer;
  if (context_provider && context_provider->BindToCurrentThread()) {
    buffer =
        CreateDrawingBuffer(std::move(context_provider), using_gpu_compositing);
  }
  if (!buffer) {
    if (context_lost_mode_ == kRealLostContext) {
      restore_timer_.StartOneShot(kDurationBetweenRestoreAttempts, FROM_HERE);
    } else {
      SynthesizeGLError(GL_INVALID_OPERATION, "", "error restoring context");
    }
    return;
  }

  drawing_buffer_ = std::move(buffer);
  GetDrawingBuffer()->Bind(GL_FRAMEBUFFER);
  lost_context_errors_.clear();
  context_lost_mode_ = kNotLostContext;
  auto_recovery_method_ = kManual;
  restore_allowed_ = false;
  RemoveFromEvictedList(this);

  SetupFlags();
  InitializeNewContext();
  MarkContextChanged(kCanvasContextChanged);
  WebGLContextEvent* event =
      WebGLContextEvent::Create(event_type_names::kWebglcontextrestored, "");
  Host()->HostDispatchEvent(event);
}
