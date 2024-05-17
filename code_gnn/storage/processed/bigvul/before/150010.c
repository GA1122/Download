void LayerTreeHostImpl::ResetTreesForTesting() {
  if (active_tree_)
    active_tree_->DetachLayers();
  active_tree_ =
      base::MakeUnique<LayerTreeImpl>(this, active_tree()->page_scale_factor(),
                                      active_tree()->top_controls_shown_ratio(),
                                      active_tree()->elastic_overscroll());
  active_tree_->property_trees()->is_active = true;
  if (pending_tree_)
    pending_tree_->DetachLayers();
  pending_tree_ = nullptr;
  pending_tree_duration_timer_ = nullptr;
  if (recycle_tree_)
    recycle_tree_->DetachLayers();
  recycle_tree_ = nullptr;
}
