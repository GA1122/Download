String WebGL2RenderingContextBase::getActiveUniformBlockName(
    WebGLProgram* program,
    GLuint uniform_block_index) {
  if (isContextLost() ||
      !ValidateWebGLObject("getActiveUniformBlockName", program))
    return String();

  if (!ValidateUniformBlockIndex("getActiveUniformBlockName", program,
                                 uniform_block_index))
    return String();

  GLint max_name_length = -1;
  ContextGL()->GetProgramiv(ObjectOrZero(program),
                            GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
                            &max_name_length);
  if (max_name_length <= 0) {
    SynthesizeGLError(GL_INVALID_VALUE, "getActiveUniformBlockName",
                      "invalid uniform block index");
    return String();
  }
  auto name = std::make_unique<GLchar[]>(max_name_length);

  GLsizei length = 0;
  ContextGL()->GetActiveUniformBlockName(ObjectOrZero(program),
                                         uniform_block_index, max_name_length,
                                         &length, name.get());

  return String(name.get(), length);
}
