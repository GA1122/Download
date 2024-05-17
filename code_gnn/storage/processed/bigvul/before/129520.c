bool GLES2DecoderImpl::WasContextLostByRobustnessExtension() {
  return WasContextLost() && reset_by_robustness_extension_;
}
