error::Error GLES2DecoderPassthroughImpl::DoViewport(GLint x,
                                                     GLint y,
                                                     GLsizei width,
                                                     GLsizei height) {
  CheckErrorCallbackState();

  gfx::Vector2d viewport_offset = GetSurfaceDrawOffset();
  api()->glViewportFn(x + viewport_offset.x(), y + viewport_offset.y(), width,
                      height);

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  AssignGLRectangle(viewport_, x, y, width, height);

  return error::kNoError;
}
