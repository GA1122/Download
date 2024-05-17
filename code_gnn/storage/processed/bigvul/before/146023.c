bool WebGL2RenderingContextBase::ValidateBufferBaseTarget(
    const char* function_name,
    GLenum target) {
  switch (target) {
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
      return true;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
      return false;
  }
}
