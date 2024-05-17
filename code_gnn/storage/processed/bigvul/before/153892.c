bool GLES2DecoderImpl::CheckDrawingFeedbackLoopsHelper(
    const Framebuffer::Attachment* attachment,
    TextureRef* texture_ref,
    const char* function_name) {
  if (attachment && attachment->IsTexture(texture_ref)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name,
        "Source and destination textures of the draw are the same.");
    return true;
  }
  return false;
}
