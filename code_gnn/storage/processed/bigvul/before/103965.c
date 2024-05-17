void GLES2DecoderImpl::ClearUnclearedRenderbuffers(
    GLenum target, FramebufferManager::FramebufferInfo* info) {
  if (target == GL_READ_FRAMEBUFFER_EXT) {
  }
  GLbitfield clear_bits = 0;
  if (info->HasUnclearedAttachment(GL_COLOR_ATTACHMENT0)) {
    glClearColor(
        0, 0, 0,
        (GLES2Util::GetChannelsForFormat(
             info->GetColorAttachmentFormat()) & 0x0008) != 0 ? 0 : 1);
    glColorMask(true, true, true, true);
    clear_bits |= GL_COLOR_BUFFER_BIT;
  }

  if (info->HasUnclearedAttachment(GL_STENCIL_ATTACHMENT) ||
      info->HasUnclearedAttachment(GL_DEPTH_STENCIL_ATTACHMENT)) {
    glClearStencil(0);
    glStencilMask(-1);
    clear_bits |= GL_STENCIL_BUFFER_BIT;
  }

  if (info->HasUnclearedAttachment(GL_DEPTH_ATTACHMENT) ||
      info->HasUnclearedAttachment(GL_DEPTH_STENCIL_ATTACHMENT)) {
    glClearDepth(1.0f);
    glDepthMask(true);
    clear_bits |= GL_DEPTH_BUFFER_BIT;
  }

  glDisable(GL_SCISSOR_TEST);
  glClear(clear_bits);

  info->MarkAttachedRenderbuffersAsCleared();

  RestoreClearState();

  if (target == GL_READ_FRAMEBUFFER_EXT) {
  }
}
