bool WebGL2RenderingContextBase::ValidateTransformFeedbackPrimitiveMode(
    const char* function_name,
    GLenum primitive_mode) {
  switch (primitive_mode) {
    case GL_POINTS:
    case GL_LINES:
    case GL_TRIANGLES:
      return true;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name,
                        "invalid transform feedback primitive mode");
      return false;
  }
}
