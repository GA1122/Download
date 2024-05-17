void WebGLRenderingContextBase::getContextAttributes(
    Nullable<WebGLContextAttributes>& result) {
  if (isContextLost())
    return;
  result.Set(ToWebGLContextAttributes(CreationAttributes()));
  if (CreationAttributes().depth() && !GetDrawingBuffer()->HasDepthBuffer())
    result.Get().setDepth(false);
  if (CreationAttributes().stencil() && !GetDrawingBuffer()->HasStencilBuffer())
    result.Get().setStencil(false);
  result.Get().setAntialias(GetDrawingBuffer()->Multisample());
}
