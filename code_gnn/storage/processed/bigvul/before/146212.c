void WebGL2RenderingContextBase::uniformBlockBinding(
    WebGLProgram* program,
    GLuint uniform_block_index,
    GLuint uniform_block_binding) {
  if (isContextLost() || !ValidateWebGLObject("uniformBlockBinding", program))
    return;

  if (!ValidateUniformBlockIndex("uniformBlockBinding", program,
                                 uniform_block_index))
    return;

  ContextGL()->UniformBlockBinding(ObjectOrZero(program), uniform_block_index,
                                   uniform_block_binding);
}
