WebGLRenderingContextBase* WebGLRenderingContextBase::OldestContext() {
  if (ActiveContexts().IsEmpty())
    return nullptr;

  WebGLRenderingContextBase* candidate = *(ActiveContexts().begin());
  DCHECK(!candidate->isContextLost());
  for (WebGLRenderingContextBase* context : ActiveContexts()) {
    DCHECK(!context->isContextLost());
    if (context->ContextGL()->GetLastFlushIdCHROMIUM() <
        candidate->ContextGL()->GetLastFlushIdCHROMIUM()) {
      candidate = context;
    }
  }

  return candidate;
}
