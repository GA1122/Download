void GLES2DecoderImpl::ClearRealGLErrors() {
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    if (error != GL_OUT_OF_MEMORY) {
      NOTREACHED() << "GL error " << error << " was unhandled.";
    }
  }
}
