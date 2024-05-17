void GLES2DecoderImpl::DoBufferSubData(
  GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) {
  BufferManager::BufferInfo* info = GetBufferInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glBufferSubData", "unknown buffer");
    return;
  }
  if (!info->SetRange(offset, size, data)) {
    SetGLError(GL_INVALID_VALUE, "glBufferSubData", "out of range");
    return;
  }
  if (bufferdata_faster_than_buffersubdata_ &&
      offset == 0 && size == info->size()) {
    glBufferData(target, size, data, info->usage());
    return;
  }
  glBufferSubData(target, offset, size, data);
}
