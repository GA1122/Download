bool GLES2DecoderImpl::DoIsValuebufferCHROMIUM(GLuint client_id) {
  const Valuebuffer* valuebuffer = GetValuebuffer(client_id);
  return valuebuffer && valuebuffer->IsValid();
}
