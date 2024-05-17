void RenderFrameHostImpl::OnRenderProcessGone(int status, int exit_code) {
  if (frame_tree_node_->IsMainFrame()) {
    render_view_host_->render_view_termination_status_ =
        static_cast<base::TerminationStatus>(status);
  }

  frame_tree_node_->ResetForNewProcess();

  SetRenderFrameCreated(false);
  InvalidateMojoConnection();

  for (const auto& iter : ax_tree_snapshot_callbacks_)
    iter.second.Run(ui::AXTreeUpdate());

  for (const auto& iter : smart_clip_callbacks_)
    iter.second.Run(base::string16(), base::string16());

  ax_tree_snapshot_callbacks_.clear();
  smart_clip_callbacks_.clear();
  javascript_callbacks_.clear();
  visual_state_callbacks_.clear();

  remote_associated_interfaces_.reset();

  if (!is_active()) {
    OnSwappedOut();
  } else {
    frame_tree_node_->render_manager()->CancelPendingIfNecessary(this);
  }

}
