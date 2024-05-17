void GLES2DecoderImpl::DoConsumeTextureCHROMIUM(GLenum target,
                                                const GLbyte* mailbox) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glConsumeTextureCHROMIUM", "unknown texture for target");
    return;
  }

  scoped_ptr<TextureDefinition> definition(
      group_->mailbox_manager()->ConsumeTexture(
      target,
      *reinterpret_cast<const MailboxName*>(mailbox)));
  if (!definition.get()) {
    SetGLError(GL_INVALID_OPERATION,
               "glConsumeTextureCHROMIUM", "invalid mailbox name");
    return;
  }

  if (!texture_manager()->Restore(info, definition.release())) {
    SetGLError(GL_INVALID_OPERATION,
               "glConsumeTextureCHROMIUM", "invalid texture");
    return;
  }

  BindAndApplyTextureParameters(info);
}
