bool WebGL2RenderingContextBase::ValidateUniformBlockIndex(
    const char* function_name,
    WebGLProgram* program,
    GLuint block_index) {
  DCHECK(program);
  if (!program->LinkStatus(this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "program not linked");
    return false;
  }
  GLint active_uniform_blocks = 0;
  ContextGL()->GetProgramiv(ObjectOrZero(program), GL_ACTIVE_UNIFORM_BLOCKS,
                            &active_uniform_blocks);
  if (block_index >= static_cast<GLuint>(active_uniform_blocks)) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "invalid uniform block index");
    return false;
  }
  return true;
}
