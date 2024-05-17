void GLES2DecoderImpl::ReleaseNotInUseBackTextures() {
  for (auto& saved_back_texture : saved_back_textures_) {
    if (!saved_back_texture.in_use)
      saved_back_texture.back_texture->Destroy();
  }

  base::EraseIf(saved_back_textures_,
                [](const SavedBackTexture& saved_back_texture) {
                  return !saved_back_texture.in_use;
                });
}
