TextureManager::~TextureManager() {
  DCHECK(texture_infos_.empty());

  CHECK_EQ(texture_info_count_, 0u);

  DCHECK_EQ(0, num_unrenderable_textures_);
  DCHECK_EQ(0, num_unsafe_textures_);
  DCHECK_EQ(0, num_uncleared_mips_);
}
