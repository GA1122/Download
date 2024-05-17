GLenum WebGLRenderingContextBase::getError() {
  if (!lost_context_errors_.IsEmpty()) {
    GLenum error = lost_context_errors_.front();
    lost_context_errors_.EraseAt(0);
    return error;
  }

  if (isContextLost())
    return GL_NO_ERROR;

  if (!synthetic_errors_.IsEmpty()) {
    GLenum error = synthetic_errors_.front();
    synthetic_errors_.EraseAt(0);
    return error;
  }

  return ContextGL()->GetError();
}
