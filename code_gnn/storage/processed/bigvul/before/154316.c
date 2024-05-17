void GLES2DecoderImpl::OnAbstractTextureDestroyed(
    ValidatingAbstractTextureImpl* abstract_texture,
    scoped_refptr<TextureRef> texture_ref) {
  DCHECK(texture_ref);
  abstract_textures_.erase(abstract_texture);
  if (context_->IsCurrent(nullptr))
    texture_refs_pending_destruction_.clear();
  else
    texture_refs_pending_destruction_.insert(std::move(texture_ref));
}
