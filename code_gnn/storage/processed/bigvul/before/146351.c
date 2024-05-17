void WebGLRenderingContextBase::RemoveFromEvictedList(
    WebGLRenderingContextBase* context) {
  ForciblyEvictedContexts().erase(context);
}
