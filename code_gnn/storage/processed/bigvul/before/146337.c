WebGLRenderingContextBase* WebGLRenderingContextBase::OldestEvictedContext() {
  if (ForciblyEvictedContexts().IsEmpty())
    return nullptr;

  WebGLRenderingContextBase* candidate = nullptr;
  int generation = -1;
  for (WebGLRenderingContextBase* context : ForciblyEvictedContexts().Keys()) {
    if (!candidate || ForciblyEvictedContexts().at(context) < generation) {
      candidate = context;
      generation = ForciblyEvictedContexts().at(context);
    }
  }

  return candidate;
}
