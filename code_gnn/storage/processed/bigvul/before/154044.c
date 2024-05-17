void GLES2DecoderImpl::DoOverlayPromotionHintCHROMIUM(GLuint client_id,
                                                      GLboolean promotion_hint,
                                                      GLint display_x,
                                                      GLint display_y,
                                                      GLint display_width,
                                                      GLint display_height) {
  if (client_id == 0)
    return;

  TextureRef* texture_ref = GetTexture(client_id);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glOverlayPromotionHintCHROMIUM",
                       "invalid texture id");
    return;
  }
  GLStreamTextureImage* image =
      texture_ref->texture()->GetLevelStreamTextureImage(
          GL_TEXTURE_EXTERNAL_OES, 0);
  if (!image) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glOverlayPromotionHintCHROMIUM",
                       "texture has no StreamTextureImage");
    return;
  }

  image->NotifyPromotionHint(promotion_hint != GL_FALSE, display_x, display_y,
                             display_width, display_height);
}
