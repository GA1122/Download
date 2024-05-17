void WebGLRenderingContextBase::LoseContextImpl(
    WebGLRenderingContextBase::LostContextMode mode,
    AutoRecoveryMethod auto_recovery_method) {
  if (isContextLost())
    return;

  context_lost_mode_ = mode;
  DCHECK_NE(context_lost_mode_, kNotLostContext);
  auto_recovery_method_ = auto_recovery_method;

  for (ExtensionTracker* tracker : extensions_) {
    tracker->LoseExtension(false);
  }

  for (wtf_size_t i = 0; i < kWebGLExtensionNameCount; ++i)
    extension_enabled_[i] = false;

  RemoveAllCompressedTextureFormats();

  if (mode == kRealLostContext) {
    task_runner_->PostTask(
        FROM_HERE,
        WTF::Bind(&WebGLRenderingContextBase::HoldReferenceToDrawingBuffer,
                  WrapWeakPersistent(this), WTF::RetainedRef(drawing_buffer_)));
  }

  DestroyContext();

  ConsoleDisplayPreference display =
      (mode == kRealLostContext) ? kDisplayInConsole : kDontDisplayInConsole;
  SynthesizeGLError(GC3D_CONTEXT_LOST_WEBGL, "loseContext", "context lost",
                    display);

  restore_allowed_ = false;
  DeactivateContext(this);
  if (auto_recovery_method_ == kWhenAvailable)
    AddToEvictedList(this);

  dispatch_context_lost_event_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}