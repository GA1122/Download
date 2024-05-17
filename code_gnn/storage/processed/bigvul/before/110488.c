bool GLES2DecoderImpl::DoIsBuffer(GLuint client_id) {
  const BufferManager::BufferInfo* buffer = GetBufferInfo(client_id);
  return buffer && buffer->IsValid() && !buffer->IsDeleted();
}
