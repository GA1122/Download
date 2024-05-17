error::Error GLES2DecoderPassthroughImpl::DoLoseContextCHROMIUM(GLenum current,
                                                                GLenum other) {
  if (!ValidContextLostReason(current) || !ValidContextLostReason(other)) {
    InsertError(GL_INVALID_ENUM, "invalid context loss reason.");
    return error::kNoError;
  }

  MarkContextLost(GetContextLostReasonFromResetStatus(current));
  group_->LoseContexts(GetContextLostReasonFromResetStatus(other));
  reset_by_robustness_extension_ = true;
  return error::kNoError;
}
