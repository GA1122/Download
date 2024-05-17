void GLES2Implementation::DeleteTexturesHelper(GLsizei n,
                                               const GLuint* textures) {
  if (!GetIdHandler(SharedIdNamespaces::kTextures)
           ->FreeIds(this, n, textures,
                     &GLES2Implementation::DeleteTexturesStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteTextures",
               "id not created by this context.");
    return;
  }
  for (GLsizei ii = 0; ii < n; ++ii) {
    share_group_->discardable_texture_manager()->FreeTexture(textures[ii]);
  }
  UnbindTexturesHelper(n, textures);
}
