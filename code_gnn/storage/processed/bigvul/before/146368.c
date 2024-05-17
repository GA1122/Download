  explicit ScopedUnpackParametersResetRestore(
      WebGLRenderingContextBase* context,
      bool enabled = true)
      : context_(context), enabled_(enabled) {
    if (enabled)
      context_->ResetUnpackParameters();
  }
