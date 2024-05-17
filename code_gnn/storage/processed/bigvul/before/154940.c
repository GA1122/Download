void WebGLRenderingContextBase::DrawingBufferClientRestoreScissorTest() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  if (scissor_enabled_)
    ContextGL()->Enable(GL_SCISSOR_TEST);
  else
    ContextGL()->Disable(GL_SCISSOR_TEST);
}
