void FeatureInfo::EnableES3Validators() {
  DCHECK(IsES3Capable());
  validators_.UpdateValuesES3();

  GLint max_color_attachments = 0;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
  const int kTotalColorAttachmentEnums = 16;
  const GLenum kColorAttachments[] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
    GL_COLOR_ATTACHMENT10,
    GL_COLOR_ATTACHMENT11,
    GL_COLOR_ATTACHMENT12,
    GL_COLOR_ATTACHMENT13,
    GL_COLOR_ATTACHMENT14,
    GL_COLOR_ATTACHMENT15,
  };
  if (max_color_attachments < kTotalColorAttachmentEnums) {
    validators_.attachment.RemoveValues(
        kColorAttachments + max_color_attachments,
        kTotalColorAttachmentEnums - max_color_attachments);
    validators_.attachment_query.RemoveValues(
        kColorAttachments + max_color_attachments,
        kTotalColorAttachmentEnums - max_color_attachments);
    validators_.read_buffer.RemoveValues(
        kColorAttachments + max_color_attachments,
        kTotalColorAttachmentEnums - max_color_attachments);
  }

  GLint max_draw_buffers = 0;
  glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_draw_buffers);
  const int kTotalDrawBufferEnums = 16;
  const GLenum kDrawBuffers[] = {
    GL_DRAW_BUFFER0,
    GL_DRAW_BUFFER1,
    GL_DRAW_BUFFER2,
    GL_DRAW_BUFFER3,
    GL_DRAW_BUFFER4,
    GL_DRAW_BUFFER5,
    GL_DRAW_BUFFER6,
    GL_DRAW_BUFFER7,
    GL_DRAW_BUFFER8,
    GL_DRAW_BUFFER9,
    GL_DRAW_BUFFER10,
    GL_DRAW_BUFFER11,
    GL_DRAW_BUFFER12,
    GL_DRAW_BUFFER13,
    GL_DRAW_BUFFER14,
    GL_DRAW_BUFFER15,
  };
  if (max_draw_buffers < kTotalDrawBufferEnums) {
    validators_.g_l_state.RemoveValues(
        kDrawBuffers + max_draw_buffers,
        kTotalDrawBufferEnums - max_draw_buffers);
  }

  if (feature_flags_.ext_texture_format_bgra8888) {
    validators_.texture_internal_format.AddValue(GL_BGRA8_EXT);
    validators_.texture_sized_color_renderable_internal_format.AddValue(
        GL_BGRA8_EXT);
    validators_.texture_sized_texture_filterable_internal_format.AddValue(
        GL_BGRA8_EXT);
  }

  if (!IsWebGLContext()) {
    validators_.texture_parameter.AddValue(GL_TEXTURE_SWIZZLE_R);
    validators_.texture_parameter.AddValue(GL_TEXTURE_SWIZZLE_G);
    validators_.texture_parameter.AddValue(GL_TEXTURE_SWIZZLE_B);
    validators_.texture_parameter.AddValue(GL_TEXTURE_SWIZZLE_A);
  }
}
