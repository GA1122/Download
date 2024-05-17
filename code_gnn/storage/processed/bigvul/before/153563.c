void GLES2Implementation::BindBufferRangeStub(GLenum target,
                                              GLuint index,
                                              GLuint buffer,
                                              GLintptr offset,
                                              GLsizeiptr size) {
  helper_->BindBufferRange(target, index, buffer, offset, size);
  if (share_group_->bind_generates_resource())
    helper_->CommandBufferHelper::Flush();
}
