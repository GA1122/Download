void WebGLRenderingContextBase::InitializeWebGLContextLimits(
    WebGraphicsContext3DProvider* context_provider) {
  MutexLocker locker(WebGLContextLimitMutex());
  if (!webgl_context_limits_initialized_) {
    auto webgl_preferences =
        context_provider->GetGpuFeatureInfo().webgl_preferences;
    max_active_webgl_contexts_ = webgl_preferences.max_active_webgl_contexts;
    max_active_webgl_contexts_on_worker_ =
        webgl_preferences.max_active_webgl_contexts_on_worker;
    webgl_context_limits_initialized_ = true;
  }
}
