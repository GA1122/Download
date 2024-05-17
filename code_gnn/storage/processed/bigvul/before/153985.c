void GLES2DecoderImpl::DoCreateAndTexStorage2DSharedImageINTERNAL(
    GLuint client_id,
    GLenum internal_format,
    const volatile GLbyte* mailbox_data) {
  TRACE_EVENT2("gpu",
               "GLES2DecoderImpl::DoCreateAndTexStorage2DSharedImageCHROMIUM",
               "context", logger_.GetLogPrefix(), "mailbox[0]",
               static_cast<unsigned char>(mailbox_data[0]));
  Mailbox mailbox = Mailbox::FromVolatile(
      *reinterpret_cast<const volatile Mailbox*>(mailbox_data));
  DLOG_IF(ERROR, !mailbox.Verify())
      << "DoCreateAndTexStorage2DSharedImageCHROMIUM was passed an invalid "
         "mailbox.";
  if (!client_id) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "DoCreateAndTexStorage2DSharedImageINTERNAL",
                       "invalid client id");
    return;
  }

  TextureRef* texture_ref = GetTexture(client_id);
  if (texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "DoCreateAndTexStorage2DSharedImageINTERNAL",
                       "client id already in use");
    return;
  }

  std::unique_ptr<SharedImageRepresentationGLTexture> shared_image;
  if (internal_format == GL_RGB) {
    shared_image = group_->shared_image_representation_factory()
                       ->ProduceRGBEmulationGLTexture(mailbox);
  } else if (internal_format == GL_NONE) {
    shared_image =
        group_->shared_image_representation_factory()->ProduceGLTexture(
            mailbox);
  } else {
    LOCAL_SET_GL_ERROR(GL_INVALID_ENUM,
                       "DoCreateAndTexStorage2DSharedImageINTERNAL",
                       "invalid internal format");
    return;
  }

  if (!shared_image) {
    bool result = GenTexturesHelper(1, &client_id);
    DCHECK(result);
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "DoCreateAndTexStorage2DSharedImageINTERNAL",
                       "invalid mailbox name");
    return;
  }

  texture_ref =
      texture_manager()->ConsumeSharedImage(client_id, std::move(shared_image));
}
