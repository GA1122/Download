void GLES2DecoderImpl::DoDidUseTexImageIfNeeded(
    Texture* texture, GLenum textarget) {
  if (texture && !texture->IsAttachedToFramebuffer()) {
    gfx::GLImage* image = texture->GetLevelImage(textarget, 0);
    if (image) {
      ScopedGLErrorSuppressor suppressor(
          "GLES2DecoderImpl::DoDidUseTexImageIfNeeded",
          GetErrorState());
      glBindTexture(textarget, texture->service_id());
      image->DidUseTexImage();
      RestoreCurrentTextureBindings(&state_, textarget);
    }
  }
}
