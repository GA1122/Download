bool WebGLRenderingContextBase::ValidateBlendFuncFactors(
    const char* function_name,
    GLenum src,
    GLenum dst) {
  if (((src == GL_CONSTANT_COLOR || src == GL_ONE_MINUS_CONSTANT_COLOR) &&
       (dst == GL_CONSTANT_ALPHA || dst == GL_ONE_MINUS_CONSTANT_ALPHA)) ||
      ((dst == GL_CONSTANT_COLOR || dst == GL_ONE_MINUS_CONSTANT_COLOR) &&
       (src == GL_CONSTANT_ALPHA || src == GL_ONE_MINUS_CONSTANT_ALPHA))) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "incompatible src and dst");
    return false;
  }
  return true;
}
