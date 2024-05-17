void GLES2DecoderImpl::OnContextLostError() {
  if (!WasContextLost()) {
    CheckResetStatus();
    group_->LoseContexts(error::kUnknown);
    reset_by_robustness_extension_ = true;
  }
}
