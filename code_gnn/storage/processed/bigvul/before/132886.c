  void SetupTrees(
      scoped_refptr<PicturePileImpl> pending_pile,
      scoped_refptr<PicturePileImpl> active_pile) {
    SetupPendingTree(active_pile);
    ActivateTree();
    SetupPendingTreeInternal(pending_pile, gfx::Size(), Region());
  }
