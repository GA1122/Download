error::Error GLES2DecoderImpl::HandlePostSubBufferCHROMIUM(
    uint32 immediate_data_size, const gles2::PostSubBufferCHROMIUM& c) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandlePostSubBufferCHROMIUM");
  if (!surface_->HasExtension("GL_CHROMIUM_post_sub_buffer")) {
    SetGLError(GL_INVALID_OPERATION,
               "glPostSubBufferCHROMIUM", "command not supported by surface");
    return error::kNoError;
  }
  if (surface_->PostSubBuffer(c.x, c.y, c.width, c.height)) {
    return error::kNoError;
  } else {
    LOG(ERROR) << "Context lost because PostSubBuffer failed.";
    return error::kLostContext;
  }
}
