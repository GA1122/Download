void LayerTreeHost::SetNeedsFullTreeSync() {
  needs_full_tree_sync_ = true;
  needs_meta_info_recomputation_ = true;

  property_trees_.needs_rebuild = true;
  SetNeedsCommit();
}
