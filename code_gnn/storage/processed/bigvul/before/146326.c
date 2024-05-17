void WebGLRenderingContextBase::LoseContext(LostContextMode mode) {
  ForceLostContext(mode, kManual);
}
