bool GLES2DecoderImpl::DoIsRenderbuffer(GLuint client_id) {
  const Renderbuffer* renderbuffer =
      GetRenderbuffer(client_id);
  return renderbuffer && renderbuffer->IsValid() && !renderbuffer->IsDeleted();
}
