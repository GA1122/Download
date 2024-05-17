bool GLES2DecoderImpl::DoIsEnabled(GLenum cap) {
  switch (cap) {
    case GL_BLEND:
      return enable_blend_;
    case GL_CULL_FACE:
      return enable_cull_face_;
    case GL_SCISSOR_TEST:
      return enable_scissor_test_;
    case GL_DEPTH_TEST:
      return enable_depth_test_;
    case GL_STENCIL_TEST:
      return enable_stencil_test_;
    default:
      return glIsEnabled(cap) != 0;
  }
}
