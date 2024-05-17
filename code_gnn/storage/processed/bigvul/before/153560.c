void GLES2Implementation::BindBufferBaseStub(GLenum target,
                                             GLuint index,
                                             GLuint buffer) {
  helper_->BindBufferBase(target, index, buffer);
  if (share_group_->bind_generates_resource())
    helper_->CommandBufferHelper::Flush();
}
