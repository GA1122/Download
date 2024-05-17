void GLES2DecoderImpl::DoWillUseTexImageIfNeeded(
    Texture* texture, GLenum textarget) {
  if (texture && !texture->IsAttachedToFramebuffer()) {
    gfx::GLImage* image = texture->GetLevelImage(textarget, 0);
    if (image) {
      ScopedGLErrorSuppressor suppressor(
          "GLES2DecoderImpl::DoWillUseTexImageIfNeeded",
          GetErrorState());
      glBindTexture(textarget, texture->service_id());
      image->WillUseTexImage();
      RestoreCurrentTextureBindings(&state_, textarget);
    }
  }
}
