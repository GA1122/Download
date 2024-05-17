WebGLRenderingContextBaseSet& ActiveContexts() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      ThreadSpecific<Persistent<WebGLRenderingContextBaseSet>>, active_contexts,
      ());
  Persistent<WebGLRenderingContextBaseSet>& active_contexts_persistent =
      *active_contexts;
  if (!active_contexts_persistent) {
    active_contexts_persistent = new WebGLRenderingContextBaseSet();
    active_contexts_persistent.RegisterAsStaticReference();
  }
  return *active_contexts_persistent;
}
