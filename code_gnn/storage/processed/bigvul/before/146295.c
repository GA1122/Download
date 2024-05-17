void WebGLRenderingContextBase::ForceLostContext(
    LostContextMode mode,
    AutoRecoveryMethod auto_recovery_method) {
  if (isContextLost()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "loseContext",
                      "context already lost");
    return;
  }

  context_group_->LoseContextGroup(mode, auto_recovery_method);
}
