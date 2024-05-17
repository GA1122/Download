error::Error GLES2DecoderPassthroughImpl::DoSetEnableDCLayersCHROMIUM(
    GLboolean enable) {
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

  if (!surface_->SetEnableDCLayers(!!enable)) {
    InsertError(GL_INVALID_OPERATION, "SetEnableDCLayers failed on surface.");
    return error::kNoError;
  }

  return error::kNoError;
}
