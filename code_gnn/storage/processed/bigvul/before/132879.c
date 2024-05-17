  void SetupDefaultTreesWithFixedTileSize(const gfx::Size& layer_bounds,
                                          const gfx::Size& tile_size,
                                          const Region& invalidation) {
    gfx::Size pile_tile_size(100, 100);

    scoped_refptr<FakePicturePileImpl> pending_pile =
        FakePicturePileImpl::CreateFilledPile(pile_tile_size, layer_bounds);
    scoped_refptr<FakePicturePileImpl> active_pile =
        FakePicturePileImpl::CreateFilledPile(pile_tile_size, layer_bounds);

    SetupTreesWithFixedTileSize(pending_pile, active_pile, tile_size,
                                invalidation);
  }
