bool GLES2DecoderImpl::DoIsRenderbuffer(GLuint client_id) {
  const RenderbufferManager::RenderbufferInfo* renderbuffer =
      GetRenderbufferInfo(client_id);
  return renderbuffer && renderbuffer->IsValid() && !renderbuffer->IsDeleted();
}
