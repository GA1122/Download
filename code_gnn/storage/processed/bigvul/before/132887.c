  void SetupTreesWithFixedTileSize(scoped_refptr<PicturePileImpl> pending_pile,
                                   scoped_refptr<PicturePileImpl> active_pile,
                                   const gfx::Size& tile_size,
                                   const Region& pending_invalidation) {
    SetupPendingTreeInternal(active_pile, tile_size, Region());
    ActivateTree();
    SetupPendingTreeInternal(pending_pile, tile_size, pending_invalidation);
  }
