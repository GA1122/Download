void ClientControlledShellSurface::CommitPendingScale() {
  if (pending_scale_ != scale_) {
    gfx::Transform transform;
    DCHECK_NE(pending_scale_, 0.0);
    transform.Scale(1.0 / pending_scale_, 1.0 / pending_scale_);
    host_window()->SetTransform(transform);
    scale_ = pending_scale_;
  }
}
