void GLES2DecoderImpl::DoProduceTextureDirectCHROMIUM(GLuint client_id,
    GLenum target, const GLbyte* data) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoProduceTextureDirectCHROMIUM",
      "context", logger_.GetLogPrefix(),
      "mailbox[0]", static_cast<unsigned char>(data[0]));

  ProduceTextureRef("glProduceTextureDirectCHROMIUM", GetTexture(client_id),
      target, data);
}
