void GLES2DecoderImpl::ReleaseAllBackTextures(bool have_context) {
  for (auto& saved_back_texture : saved_back_textures_) {
    if (have_context)
      saved_back_texture.back_texture->Destroy();
    else
      saved_back_texture.back_texture->Invalidate();
  }
  saved_back_textures_.clear();
}
