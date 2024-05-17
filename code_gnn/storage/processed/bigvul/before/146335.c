void WebGLRenderingContextBase::MaybeRestoreContext(TimerBase*) {
  DCHECK(isContextLost());

  if (!restore_allowed_)
    return;

  if (canvas()) {
    LocalFrame* frame = canvas()->GetDocument().GetFrame();
    if (!frame)
      return;

    Settings* settings = frame->GetSettings();

    if (!frame->Client()->AllowWebGL(settings && settings->GetWebGLEnabled()))
      return;
  }

  if (GetDrawingBuffer()) {
    drawing_buffer_->BeginDestruction();
    drawing_buffer_.Clear();
  }

  auto execution_context = host()->GetTopExecutionContext();
  Platform::ContextAttributes attributes = ToPlatformContextAttributes(
      CreationAttributes(), Version(),
      SupportOwnOffscreenSurface(execution_context));
  Platform::GraphicsInfo gl_info;
  std::unique_ptr<WebGraphicsContext3DProvider> context_provider;
  const auto& url = host()->GetExecutionContextUrl();

  if (IsMainThread()) {
    context_provider =
        Platform::Current()->CreateOffscreenGraphicsContext3DProvider(
            attributes, url, 0, &gl_info);
  } else {
    context_provider =
        CreateContextProviderOnWorkerThread(attributes, &gl_info, url);
  }
  RefPtr<DrawingBuffer> buffer;
  if (context_provider && context_provider->BindToCurrentThread()) {
    buffer = CreateDrawingBuffer(std::move(context_provider));
  }
  if (!buffer) {
    if (context_lost_mode_ == kRealLostContext) {
      restore_timer_.StartOneShot(kSecondsBetweenRestoreAttempts,
                                  BLINK_FROM_HERE);
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
  WebGLContextEvent* event = WebGLContextEvent::Create(
      EventTypeNames::webglcontextrestored, false, true, "");
  host()->HostDispatchEvent(event);
}
