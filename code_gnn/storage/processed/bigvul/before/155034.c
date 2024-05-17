WebGLContextAttributes* WebGLRenderingContextBase::getContextAttributes()
    const {
  if (isContextLost())
    return nullptr;

  WebGLContextAttributes* result =
      ToWebGLContextAttributes(CreationAttributes());

  if (CreationAttributes().depth && !GetDrawingBuffer()->HasDepthBuffer())
    result->setDepth(false);
  if (CreationAttributes().stencil && !GetDrawingBuffer()->HasStencilBuffer())
    result->setStencil(false);
  result->setAntialias(GetDrawingBuffer()->Multisample());
  result->setXrCompatible(xr_compatible_);
  return result;
}
