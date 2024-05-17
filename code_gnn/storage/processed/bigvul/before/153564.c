void GLES2Implementation::BindBufferStub(GLenum target, GLuint buffer) {
  helper_->BindBuffer(target, buffer);
  if (share_group_->bind_generates_resource())
    helper_->CommandBufferHelper::OrderingBarrier();
}
