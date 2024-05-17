void WebGLRenderingContextBase::Stop() {
  if (!isContextLost()) {
    ForceLostContext(kSyntheticLostContext, kManual);
  }
}
