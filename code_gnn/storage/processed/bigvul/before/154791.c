error::Error GLES2DecoderPassthroughImpl::DoScissor(GLint x,
                                                    GLint y,
                                                    GLsizei width,
                                                    GLsizei height) {
  CheckErrorCallbackState();

  gfx::Vector2d scissor_offset = GetSurfaceDrawOffset();
  api()->glScissorFn(x + scissor_offset.x(), y + scissor_offset.y(), width,
                     height);

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  AssignGLRectangle(scissor_, x, y, width, height);

  return error::kNoError;
}
