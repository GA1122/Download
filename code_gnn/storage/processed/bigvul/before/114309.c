void WebGraphicsContext3DCommandBufferImpl::flush() {
  gl_->Flush();
  if (!visible_ && free_command_buffer_when_invisible_)
    gl_->FreeEverything();
}
