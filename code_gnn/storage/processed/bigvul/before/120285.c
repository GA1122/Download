void LayerTreeHost::UpdateTopControlsState(TopControlsState constraints,
                                           TopControlsState current,
                                           bool animate) {
  if (!settings_.calculate_top_controls_position)
    return;

  proxy_->ImplThreadTaskRunner()->PostTask(
      FROM_HERE,
      base::Bind(&TopControlsManager::UpdateTopControlsState,
                 top_controls_manager_weak_ptr_,
                 constraints,
                 current,
                 animate));
}
