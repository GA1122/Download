uint32_t GLES2Util::GetChannelsNeededForAttachmentType(
    int type,
    uint32_t max_color_attachments) {
  switch (type) {
    case GL_DEPTH_ATTACHMENT:
      return kDepth;
    case GL_STENCIL_ATTACHMENT:
      return kStencil;
    default:
      if (type >= GL_COLOR_ATTACHMENT0 &&
          type < static_cast<int>(
              GL_COLOR_ATTACHMENT0 + max_color_attachments)) {
        return kRGBA;
      }
      return 0x0000;
  }
}
