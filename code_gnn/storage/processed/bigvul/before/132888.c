  void SetupTreesWithInvalidation(scoped_refptr<PicturePileImpl> pending_pile,
                                  scoped_refptr<PicturePileImpl> active_pile,
                                  const Region& pending_invalidation) {
    SetupPendingTreeInternal(active_pile, gfx::Size(), Region());
    ActivateTree();
    SetupPendingTreeInternal(pending_pile, gfx::Size(), pending_invalidation);
  }
