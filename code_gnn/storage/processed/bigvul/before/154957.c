unsigned WebGLRenderingContextBase::GetWebGLVersion(
    const CanvasRenderingContext* context) {
  if (!context->Is3d())
    return 0;
  return static_cast<const WebGLRenderingContextBase*>(context)->ContextType();
}
