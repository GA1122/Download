void WebGraphicsContext3DCommandBufferImpl::setVisibilityCHROMIUM(
    bool visible) {
  gl_->Flush();
  context_->SetSurfaceVisible(visible);
  if (!visible)
    gl_->FreeEverything();
}
