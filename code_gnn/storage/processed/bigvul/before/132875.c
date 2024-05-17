  explicit PictureLayerImplTest(const LayerTreeSettings& settings)
      : proxy_(base::MessageLoopProxy::current()),
        host_impl_(settings, &proxy_, &shared_bitmap_manager_),
        root_id_(6),
        id_(7) {
    host_impl_.SetViewportSize(gfx::Size(10000, 10000));
  }
