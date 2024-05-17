void WebGraphicsContext3DCommandBufferImpl::Destroy() {
  SetParent(NULL);

  if (gl_) {
    gl_->Flush();

    delete gl_;
    gl_ = NULL;
  }

  if (transfer_buffer_) {
    delete transfer_buffer_;
    transfer_buffer_ = NULL;
  }

  delete gles2_helper_;
  gles2_helper_ = NULL;

  if (host_ && command_buffer_) {
    host_->DestroyCommandBuffer(command_buffer_);
    command_buffer_ = NULL;
  }

   host_ = NULL;
 }
