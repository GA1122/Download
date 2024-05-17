unsigned WebGLRenderingContextBase::CurrentMaxGLContexts() {
  MutexLocker locker(WebGLContextLimitMutex());
  DCHECK(webgl_context_limits_initialized_);
  return IsMainThread() ? max_active_webgl_contexts_
                        : max_active_webgl_contexts_on_worker_;
}
