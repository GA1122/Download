void GLES2DecoderImpl::DeleteTexturesHelper(GLsizei n,
                                            const volatile GLuint* client_ids) {
  bool supports_separate_framebuffer_binds = SupportsSeparateFramebufferBinds();
  for (GLsizei ii = 0; ii < n; ++ii) {
    GLuint client_id = client_ids[ii];
    TextureRef* texture_ref = GetTexture(client_id);
    if (texture_ref) {
      UnbindTexture(texture_ref, supports_separate_framebuffer_binds);
      RemoveTexture(client_id);
    }
  }
}
