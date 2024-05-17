void RenderFrameHostImpl::OnRenderProcessGone(int status, int exit_code) {
  if (frame_tree_node_->IsMainFrame()) {
    render_view_host_->render_view_termination_status_ =
        static_cast<base::TerminationStatus>(status);
  }

  frame_tree_node_->ResetForNewProcess();

  SetRenderFrameCreated(false);
  InvalidateMojoConnection();
  document_scoped_interface_provider_binding_.Close();
  SetLastCommittedUrl(GURL());

  for (auto& iter : ax_tree_snapshot_callbacks_)
    std::move(iter.second).Run(ui::AXTreeUpdate());

#if defined(OS_ANDROID)
  for (base::IDMap<std::unique_ptr<ExtractSmartClipDataCallback>>::iterator
           iter(&smart_clip_callbacks_);
       !iter.IsAtEnd(); iter.Advance()) {
    std::move(*iter.GetCurrentValue()).Run(base::string16(), base::string16());
  }
  smart_clip_callbacks_.Clear();
#endif   

  ax_tree_snapshot_callbacks_.clear();
  javascript_callbacks_.clear();
  visual_state_callbacks_.clear();

  remote_associated_interfaces_.reset();

  sudden_termination_disabler_types_enabled_ = 0;

  if (!is_active()) {
    OnSwappedOut();
  } else {
    frame_tree_node_->render_manager()->CancelPendingIfNecessary(this);
  }

}
