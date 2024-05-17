  void ActivateTree() {
    host_impl_.ActivateSyncTree();
    CHECK(!host_impl_.pending_tree());
    CHECK(host_impl_.recycle_tree());
    old_pending_layer_ = pending_layer_;
    pending_layer_ = nullptr;
    active_layer_ = static_cast<FakePictureLayerImpl*>(
        host_impl_.active_tree()->LayerById(id_));

    host_impl_.active_tree()->UpdateDrawProperties();
  }
