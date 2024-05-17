void GLES2DecoderImpl::DoProduceTextureCHROMIUM(GLenum target,
                                                const GLbyte* data) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoProduceTextureCHROMIUM",
      "context", logger_.GetLogPrefix(),
      "mailbox[0]", static_cast<unsigned char>(data[0]));

  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  ProduceTextureRef("glProduceTextureCHROMIUM", texture_ref, target, data);
}
