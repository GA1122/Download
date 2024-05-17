void GLES2DecoderImpl::DoDisable(GLenum cap) {
  if (SetCapabilityState(cap, false)) {
    if (cap == GL_PRIMITIVE_RESTART_FIXED_INDEX &&
        feature_info_->feature_flags().emulate_primitive_restart_fixed_index) {
      return;
    }
    if (cap == GL_FRAMEBUFFER_SRGB) {
      return;
    }
    api()->glDisableFn(cap);
  }
}
