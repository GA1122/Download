void Layer::SetForceRenderSurface(bool force) {
  DCHECK(IsPropertyChangeAllowed());
  if (force_render_surface_ == force)
    return;
  force_render_surface_ = force;
  SetNeedsCommit();
}
