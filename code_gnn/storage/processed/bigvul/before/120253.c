RendererCapabilities::RendererCapabilities()
    : best_texture_format(RGBA_8888),
      allow_partial_texture_updates(false),
      using_offscreen_context3d(false),
      max_texture_size(0),
      using_shared_memory_resources(false) {}
