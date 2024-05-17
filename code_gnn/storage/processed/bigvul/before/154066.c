void GLES2DecoderImpl::DoSetEnableDCLayersCHROMIUM(GLboolean enable) {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(GL_DRAW_FRAMEBUFFER);
  if (framebuffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetEnableDCLayersCHROMIUM",
                       "framebuffer must not be bound");
    return;
  }
  if (!supports_dc_layers_) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetEnableDCLayersCHROMIUM",
                       "surface doesn't support SetDrawRectangle");
    return;
  }
  if (!surface_->SetEnableDCLayers(!!enable)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetEnableDCLayersCHROMIUM",
                       "failed on surface");
  }
}
