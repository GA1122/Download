void LayerTreeHostImpl::OnDraw(const gfx::Transform& transform,
                               const gfx::Rect& viewport,
                               bool resourceless_software_draw) {
  DCHECK(!resourceless_software_draw_);
  const bool transform_changed = external_transform_ != transform;
  const bool viewport_changed = external_viewport_ != viewport;

  external_transform_ = transform;
  external_viewport_ = viewport;

  {
    base::AutoReset<bool> resourceless_software_draw_reset(
        &resourceless_software_draw_, resourceless_software_draw);

    if (transform_changed || viewport_changed || resourceless_software_draw_) {
      SetFullViewportDamage();
      SetNeedsRedraw();
      active_tree_->set_needs_update_draw_properties();
    }

    if (resourceless_software_draw) {
      client_->OnCanDrawStateChanged(CanDraw());
    }

    client_->OnDrawForCompositorFrameSink(resourceless_software_draw_);
  }

  if (resourceless_software_draw) {
    active_tree_->set_needs_update_draw_properties();
    client_->OnCanDrawStateChanged(CanDraw());
    SetFullViewportDamage();
  }
}
