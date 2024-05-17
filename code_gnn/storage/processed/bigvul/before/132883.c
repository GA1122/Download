  void SetupPendingTreeInternal(scoped_refptr<RasterSource> raster_source,
                                const gfx::Size& tile_size,
                                const Region& invalidation) {
    host_impl_.CreatePendingTree();
    host_impl_.pending_tree()->PushPageScaleFromMainThread(1.f, 0.25f, 100.f);
    LayerTreeImpl* pending_tree = host_impl_.pending_tree();

    scoped_ptr<LayerImpl> pending_root = pending_tree->DetachLayerTree();
    scoped_ptr<FakePictureLayerImpl> pending_layer;
    DCHECK_IMPLIES(pending_root, pending_root->id() == root_id_);
    if (!pending_root) {
      pending_root = LayerImpl::Create(pending_tree, root_id_);
      pending_layer = FakePictureLayerImpl::Create(pending_tree, id_);
      if (!tile_size.IsEmpty())
        pending_layer->set_fixed_tile_size(tile_size);
      pending_layer->SetDrawsContent(true);
    } else {
      pending_layer.reset(static_cast<FakePictureLayerImpl*>(
          pending_root->RemoveChild(pending_root->children()[0]).release()));
      if (!tile_size.IsEmpty())
        pending_layer->set_fixed_tile_size(tile_size);
    }
    pending_layer->SetBounds(raster_source->GetSize());
    pending_layer->SetContentBounds(raster_source->GetSize());
    pending_layer->SetRasterSourceOnPending(raster_source, invalidation);

    pending_root->AddChild(pending_layer.Pass());
    pending_tree->SetRootLayer(pending_root.Pass());

    pending_layer_ = static_cast<FakePictureLayerImpl*>(
        host_impl_.pending_tree()->LayerById(id_));
    pending_layer_->DoPostCommitInitializationIfNeeded();

    host_impl_.pending_tree()->UpdateDrawProperties();
  }
