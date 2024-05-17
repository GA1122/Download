bool GLES2DecoderImpl::DoIsBuffer(GLuint client_id) {
  const BufferManager::BufferInfo* info = GetBufferInfo(client_id);
  return info && info->IsValid();
}
