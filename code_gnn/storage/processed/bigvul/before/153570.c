void GLES2Implementation::BindRenderbufferStub(GLenum target,
                                               GLuint renderbuffer) {
  helper_->BindRenderbuffer(target, renderbuffer);
  if (share_group_->bind_generates_resource())
    helper_->CommandBufferHelper::OrderingBarrier();
}
