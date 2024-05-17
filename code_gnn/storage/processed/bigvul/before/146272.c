void WebGLRenderingContextBase::DeactivateContext(
    WebGLRenderingContextBase* context) {
  ActiveContexts().erase(context);
}
