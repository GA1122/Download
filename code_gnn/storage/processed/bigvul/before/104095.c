GLenum GLES2DecoderImpl::GetGLError() {
  GLenum error = glGetError();
  if (error == GL_NO_ERROR && error_bits_ != 0) {
    for (uint32 mask = 1; mask != 0; mask = mask << 1) {
      if ((error_bits_ & mask) != 0) {
        error = GLES2Util::GLErrorBitToGLError(mask);
        break;
      }
    }
  }

  if (error != GL_NO_ERROR) {
    error_bits_ &= ~GLES2Util::GLErrorToErrorBit(error);
  }
  return error;
}
