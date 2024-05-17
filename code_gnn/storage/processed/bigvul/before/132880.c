  void SetupDefaultTreesWithInvalidation(const gfx::Size& layer_bounds,
                                         const Region& invalidation) {
    gfx::Size tile_size(100, 100);

    scoped_refptr<FakePicturePileImpl> pending_pile =
        FakePicturePileImpl::CreateFilledPile(tile_size, layer_bounds);
    scoped_refptr<FakePicturePileImpl> active_pile =
        FakePicturePileImpl::CreateFilledPile(tile_size, layer_bounds);

    SetupTreesWithInvalidation(pending_pile, active_pile, invalidation);
  }
