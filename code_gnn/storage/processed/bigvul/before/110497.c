void GLES2DecoderImpl::DoProduceTextureCHROMIUM(GLenum target,
                                                const GLbyte* mailbox) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glProduceTextureCHROMIUM", "unknown texture for target");
    return;
  }

  TextureDefinition* definition = texture_manager()->Save(info);
  if (!definition) {
    SetGLError(GL_INVALID_OPERATION,
               "glProduceTextureCHROMIUM", "invalid texture");
    return;
  }

  if (!group_->mailbox_manager()->ProduceTexture(
      target,
      *reinterpret_cast<const MailboxName*>(mailbox),
      definition,
      texture_manager())) {
    bool success = texture_manager()->Restore(info, definition);
    DCHECK(success);
    SetGLError(GL_INVALID_OPERATION,
               "glProduceTextureCHROMIUM", "invalid mailbox name");
    return;
  }

  BindAndApplyTextureParameters(info);
}
