std::unique_ptr<ScrollAndScaleSet> LayerTreeHostImpl::ProcessScrollDeltas() {
  std::unique_ptr<ScrollAndScaleSet> scroll_info(new ScrollAndScaleSet());

  CollectScrollDeltas(scroll_info.get(), active_tree_.get());
  CollectScrollbarUpdates(scroll_info.get(), &scrollbar_animation_controllers_);
  scroll_info->page_scale_delta =
      active_tree_->page_scale_factor()->PullDeltaForMainThread();
  scroll_info->top_controls_delta =
      active_tree()->top_controls_shown_ratio()->PullDeltaForMainThread();
  scroll_info->elastic_overscroll_delta =
      active_tree_->elastic_overscroll()->PullDeltaForMainThread();
  scroll_info->swap_promises.swap(swap_promises_for_main_thread_scroll_update_);

  return scroll_info;
}
