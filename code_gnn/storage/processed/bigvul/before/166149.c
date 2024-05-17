void RenderFrameHostImpl::OnRenderProcessGone(int status, int exit_code) {
  base::TerminationStatus termination_status =
      static_cast<base::TerminationStatus>(status);

  if (frame_tree_node_->IsMainFrame()) {
    render_view_host_->render_view_termination_status_ = termination_status;
  }

  if (base::FeatureList::IsEnabled(features::kCrashReporting))
    MaybeGenerateCrashReport(termination_status);

  ResetChildren();

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
    std::move(*iter.GetCurrentValue())
        .Run(base::string16(), base::string16(), gfx::Rect());
  }
  smart_clip_callbacks_.Clear();
#endif   

  ax_tree_snapshot_callbacks_.clear();
  javascript_callbacks_.clear();
  visual_state_callbacks_.clear();

  remote_associated_interfaces_.reset();

  sudden_termination_disabler_types_enabled_ = 0;

  if (unload_state_ != UnloadState::NotRun) {
    unload_state_ = UnloadState::Completed;
    DCHECK(children_.empty());
    PendingDeletionCheckCompleted();
    return;
  }

  frame_tree_node_->render_manager()->CancelPendingIfNecessary(this);

}
