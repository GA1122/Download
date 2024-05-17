void TextureManager::Destroy(bool have_context) {
  have_context_ = have_context;
  texture_infos_.clear();
  for (int ii = 0; ii < kNumDefaultTextures; ++ii) {
    default_textures_[ii] = NULL;
  }

  if (have_context) {
    glDeleteTextures(arraysize(black_texture_ids_), black_texture_ids_);
  }

  DCHECK_EQ(0u, mem_represented_);
  UpdateMemRepresented();
}
