void GLES2DecoderImpl::DoSetDrawRectangleCHROMIUM(GLint x,
                                                  GLint y,
                                                  GLint width,
                                                  GLint height) {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(GL_DRAW_FRAMEBUFFER);
  if (framebuffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetDrawRectangleCHROMIUM",
                       "framebuffer must not be bound");
    return;
  }
  if (!supports_dc_layers_) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetDrawRectangleCHROMIUM",
                       "surface doesn't support SetDrawRectangle");
    return;
  }
  gfx::Rect rect(x, y, width, height);
  if (!surface_->SetDrawRectangle(rect)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSetDrawRectangleCHROMIUM",
                       "failed on surface");
    LOG(ERROR) << "Context lost because SetDrawRectangleCHROMIUM failed.";
    MarkContextLost(error::kUnknown);
    group_->LoseContexts(error::kUnknown);
  }
  OnFboChanged();
}
