void ClientControlledShellSurface::OnPostWidgetCommit() {
  DCHECK(widget_);

  UpdateFrame();
  UpdateBackdrop();

  if (geometry_changed_callback_)
    geometry_changed_callback_.Run(GetVisibleBounds());

  if (pending_top_inset_height_ != top_inset_height_) {
    widget_->GetNativeWindow()->SetProperty(aura::client::kTopViewInset,
                                            pending_top_inset_height_);
    top_inset_height_ = pending_top_inset_height_;
  }

  CommitPendingScale();

  orientation_ = pending_orientation_;
  if (expected_orientation_ == orientation_)
    orientation_compositor_lock_.reset();
}
