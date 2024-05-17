  PictureLayerImplTest()
      : proxy_(base::MessageLoopProxy::current()),
        host_impl_(LowResTilingsSettings(), &proxy_, &shared_bitmap_manager_),
        root_id_(6),
        id_(7),
        pending_layer_(nullptr),
        old_pending_layer_(nullptr),
        active_layer_(nullptr) {
    host_impl_.SetViewportSize(gfx::Size(10000, 10000));
  }
