void GLES2DecoderImpl::OnTextureRefDetachedFromFramebuffer(
    TextureRef* texture_ref) {
  Texture* texture = texture_ref->texture();
  DoDidUseTexImageIfNeeded(texture, texture->target());
}
