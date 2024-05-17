error::Error GLES2DecoderPassthroughImpl::DoSetDrawRectangleCHROMIUM(
    GLint x,
    GLint y,
    GLint width,
    GLint height) {
  GLint current_framebuffer = 0;
  api()->glGetIntegervFn(GL_FRAMEBUFFER_BINDING, &current_framebuffer);
  if (current_framebuffer != 0) {
    InsertError(GL_INVALID_OPERATION, "framebuffer must not be bound.");
    return error::kNoError;
  }

  if (!surface_->SupportsDCLayers()) {
    InsertError(GL_INVALID_OPERATION,
                "surface doesn't support SetDrawRectangle.");
    return error::kNoError;
  }

  gfx::Rect rect(x, y, width, height);
  if (!surface_->SetDrawRectangle(rect)) {
    InsertError(GL_INVALID_OPERATION, "SetDrawRectangle failed on surface");
    return error::kNoError;
  }

  ApplySurfaceDrawOffset();

  return error::kNoError;
}
