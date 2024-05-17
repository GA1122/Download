void GLES2DecoderPassthroughImpl::OnAbstractTextureDestroyed(
    PassthroughAbstractTextureImpl* abstract_texture,
    scoped_refptr<TexturePassthrough> texture) {
  DCHECK(texture);
  abstract_textures_.erase(abstract_texture);
  if (context_->IsCurrent(nullptr)) {
    resources_->DestroyPendingTextures(true);
  } else {
    resources_->textures_pending_destruction.insert(std::move(texture));
  }
}
