GLuint WebGL2RenderingContextBase::getUniformBlockIndex(
    WebGLProgram* program,
    const String& uniform_block_name) {
  if (isContextLost() || !ValidateWebGLObject("getUniformBlockIndex", program))
    return 0;
  if (!ValidateString("getUniformBlockIndex", uniform_block_name))
    return 0;

  return ContextGL()->GetUniformBlockIndex(ObjectOrZero(program),
                                           uniform_block_name.Utf8().data());
}
