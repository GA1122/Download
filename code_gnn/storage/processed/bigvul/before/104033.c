bool GLES2DecoderImpl::DoIsFramebuffer(GLuint client_id) {
  const FramebufferManager::FramebufferInfo* info =
      GetFramebufferInfo(client_id);
  return info && info->IsValid();
}
