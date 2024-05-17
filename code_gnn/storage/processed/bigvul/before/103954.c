void GLES2DecoderImpl::ApplyDirtyState() {
  if (state_dirty_) {
    glColorMask(
        mask_red_, mask_green_, mask_blue_,
        mask_alpha_ && BoundFramebufferHasColorAttachmentWithAlpha());
    bool have_depth = BoundFramebufferHasDepthAttachment();
    glDepthMask(mask_depth_ && have_depth);
    EnableDisable(GL_DEPTH_TEST, enable_depth_test_ && have_depth);
    bool have_stencil = BoundFramebufferHasStencilAttachment();
    glStencilMaskSeparate(GL_FRONT, have_stencil ? mask_stencil_front_ : 0);
    glStencilMaskSeparate(GL_BACK, have_stencil ? mask_stencil_back_ : 0);
    EnableDisable(GL_STENCIL_TEST, enable_stencil_test_ && have_stencil);
    state_dirty_ = false;
  }
}
