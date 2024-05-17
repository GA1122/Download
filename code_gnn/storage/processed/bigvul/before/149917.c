void LayerTreeHostImpl::CreatePendingTree() {
  CHECK(!pending_tree_);
  if (recycle_tree_) {
    recycle_tree_.swap(pending_tree_);
  } else {
    pending_tree_ = base::MakeUnique<LayerTreeImpl>(
        this, active_tree()->page_scale_factor(),
        active_tree()->top_controls_shown_ratio(),
        active_tree()->elastic_overscroll());
  }

  client_->OnCanDrawStateChanged(CanDraw());
  TRACE_EVENT_ASYNC_BEGIN0("cc", "PendingTree:waiting", pending_tree_.get());

  DCHECK(!pending_tree_duration_timer_);
  pending_tree_duration_timer_.reset(new PendingTreeDurationHistogramTimer());
}
