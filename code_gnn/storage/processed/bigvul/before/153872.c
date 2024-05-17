void GLES2DecoderImpl::ApplyDirtyState() {
  if (framebuffer_state_.clear_state_dirty) {
    bool allows_alpha_change = BoundFramebufferAllowsChangesToAlphaChannel();
    state_.SetDeviceColorMask(state_.color_mask_red, state_.color_mask_green,
                              state_.color_mask_blue,
                              state_.color_mask_alpha && allows_alpha_change);

    bool have_depth = BoundFramebufferHasDepthAttachment();
    state_.SetDeviceDepthMask(state_.depth_mask && have_depth);

    bool have_stencil = BoundFramebufferHasStencilAttachment();
    state_.SetDeviceStencilMaskSeparate(
        GL_FRONT, have_stencil ? state_.stencil_front_writemask : 0);
    state_.SetDeviceStencilMaskSeparate(
        GL_BACK, have_stencil ? state_.stencil_back_writemask : 0);

    state_.SetDeviceCapabilityState(
        GL_DEPTH_TEST, state_.enable_flags.depth_test && have_depth);
    state_.SetDeviceCapabilityState(
        GL_STENCIL_TEST, state_.enable_flags.stencil_test && have_stencil);
    framebuffer_state_.clear_state_dirty = false;
  }
}
