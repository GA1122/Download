void LayerTreeHostImpl::InvalidateContentOnImplSide() {
  DCHECK(!pending_tree_);

  if (!CommitToActiveTree())
    CreatePendingTree();
  UpdateSyncTreeAfterCommitOrImplSideInvalidation();
}
