void WebGraphicsContext3DCommandBufferImpl::ensureFramebufferCHROMIUM() {
  gl_->Flush();
  command_buffer_->EnsureBackbuffer();
}
