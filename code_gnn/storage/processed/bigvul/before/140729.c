void GLES2DecoderImpl::DoConsumeTextureCHROMIUM(GLenum target,
                                                const GLbyte* data) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoConsumeTextureCHROMIUM",
      "context", logger_.GetLogPrefix(),
      "mailbox[0]", static_cast<unsigned char>(data[0]));
  const Mailbox& mailbox = *reinterpret_cast<const Mailbox*>(data);
  DLOG_IF(ERROR, !mailbox.Verify()) << "ConsumeTextureCHROMIUM was passed a "
                                       "mailbox that was not generated by "
                                       "GenMailboxCHROMIUM.";

  scoped_refptr<TextureRef> texture_ref =
      texture_manager()->GetTextureInfoForTargetUnlessDefault(&state_, target);
  if (!texture_ref.get()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glConsumeTextureCHROMIUM",
                       "unknown texture for target");
    return;
  }
  GLuint client_id = texture_ref->client_id();
  if (!client_id) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glConsumeTextureCHROMIUM", "unknown texture for target");
    return;
  }
  Texture* texture = group_->mailbox_manager()->ConsumeTexture(mailbox);
  if (!texture) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glConsumeTextureCHROMIUM", "invalid mailbox name");
    return;
  }
  if (texture->target() != target) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glConsumeTextureCHROMIUM", "invalid target");
    return;
  }

  DeleteTexturesHelper(1, &client_id);
  texture_ref = texture_manager()->Consume(client_id, texture);
  glBindTexture(target, texture_ref->service_id());

  TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
  unit.bind_target = target;
  switch (target) {
    case GL_TEXTURE_2D:
      unit.bound_texture_2d = texture_ref;
      break;
    case GL_TEXTURE_CUBE_MAP:
      unit.bound_texture_cube_map = texture_ref;
      break;
    case GL_TEXTURE_EXTERNAL_OES:
      unit.bound_texture_external_oes = texture_ref;
      break;
    case GL_TEXTURE_RECTANGLE_ARB:
      unit.bound_texture_rectangle_arb = texture_ref;
      break;
    default:
      NOTREACHED();   
      break;
  }
}