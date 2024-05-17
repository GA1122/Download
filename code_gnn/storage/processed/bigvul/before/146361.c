void WebGLRenderingContextBase::RestoreScissorEnabled() {
  if (isContextLost())
    return;

  if (scissor_enabled_) {
    ContextGL()->Enable(GL_SCISSOR_TEST);
  } else {
    ContextGL()->Disable(GL_SCISSOR_TEST);
  }
}
