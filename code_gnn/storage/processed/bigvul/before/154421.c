void GLES2DecoderPassthroughImpl::BindPendingImagesForSamplers() {
  for (TexturePendingBinding& pending : textures_pending_binding_)
    BindOnePendingImage(pending.target, pending.texture.get());

  textures_pending_binding_.clear();
}
