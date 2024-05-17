void GLES2DecoderImpl::DoEnable(GLenum cap) {
  if (SetCapabilityState(cap, true)) {
    if (cap == GL_PRIMITIVE_RESTART_FIXED_INDEX &&
        feature_info_->feature_flags().emulate_primitive_restart_fixed_index) {
      return;
    }
    if (cap == GL_FRAMEBUFFER_SRGB) {
      return;
    }
    api()->glEnableFn(cap);
  }
}
