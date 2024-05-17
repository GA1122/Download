error::Error GLES2DecoderPassthroughImpl::DoMultiDrawBeginCHROMIUM(
    GLsizei drawcount) {
  if (!multi_draw_manager_->Begin(drawcount)) {
    return error::kInvalidArguments;
  }
  return error::kNoError;
}
