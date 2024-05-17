void WebGraphicsContext3DCommandBufferImpl::discardFramebufferEXT(
    WGC3Denum target, WGC3Dsizei numAttachments, const WGC3Denum* attachments) {
  gl_->Flush();
  command_buffer_->DiscardBackbuffer();
}
