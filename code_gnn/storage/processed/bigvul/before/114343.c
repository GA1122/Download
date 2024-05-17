void WebGraphicsContext3DCommandBufferImpl::setVisibilityCHROMIUM(
    bool visible) {
  gl_->Flush();
  visible_ = visible;
  command_buffer_->SetSurfaceVisible(visible);
  if (!visible)
    gl_->FreeEverything();
}
