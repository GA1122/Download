WebGLRenderingContextBaseSet& ActiveContexts() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(ThreadSpecific<WebGLRenderingContextBaseSet>,
                                  active_contexts, ());
  if (!active_contexts.IsSet())
    active_contexts->RegisterAsStaticReference();
  return *active_contexts;
}
