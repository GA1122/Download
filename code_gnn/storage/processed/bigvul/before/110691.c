TextureManager::TextureManager(
    MemoryTracker* memory_tracker,
    FeatureInfo* feature_info,
    GLint max_texture_size,
    GLint max_cube_map_texture_size)
    : texture_memory_tracker_(new MemoryTypeTracker(
        memory_tracker,
        "TextureManager",
        "TextureMemory")),
      feature_info_(feature_info),
      max_texture_size_(max_texture_size),
      max_cube_map_texture_size_(max_cube_map_texture_size),
      max_levels_(ComputeMipMapCount(max_texture_size,
                                     max_texture_size,
                                     max_texture_size)),
      max_cube_map_levels_(ComputeMipMapCount(max_cube_map_texture_size,
                                              max_cube_map_texture_size,
                                              max_cube_map_texture_size)),
      num_unrenderable_textures_(0),
      num_unsafe_textures_(0),
      num_uncleared_mips_(0),
      texture_info_count_(0),
      mem_represented_(0),
      have_context_(true) {
  for (int ii = 0; ii < kNumDefaultTextures; ++ii) {
    black_texture_ids_[ii] = 0;
  }
}
