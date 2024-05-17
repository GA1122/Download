void WebGLRenderingContextBase::getContextAttributes(
    base::Optional<WebGLContextAttributes>& result) {
  if (isContextLost())
    return;
  result = ToWebGLContextAttributes(CreationAttributes());
  if (CreationAttributes().depth && !GetDrawingBuffer()->HasDepthBuffer())
    result->setDepth(false);
  if (CreationAttributes().stencil && !GetDrawingBuffer()->HasStencilBuffer())
    result->setStencil(false);
  result->setAntialias(GetDrawingBuffer()->Multisample());
  if (compatible_xr_device_) {
    result->setCompatibleXRDevice(compatible_xr_device_);
  }
}
