void LayerTreeHostImpl::SetViewportSize(const gfx::Size& device_viewport_size) {
  if (device_viewport_size == device_viewport_size_)
    return;
  TRACE_EVENT_INSTANT2("cc", "LayerTreeHostImpl::SetViewportSize",
                       TRACE_EVENT_SCOPE_THREAD, "width",
                       device_viewport_size.width(), "height",
                       device_viewport_size.height());

  if (pending_tree_)
    active_tree_->SetViewportSizeInvalid();

  device_viewport_size_ = device_viewport_size;

  UpdateViewportContainerSizes();
  client_->OnCanDrawStateChanged(CanDraw());
  SetFullViewportDamage();
  active_tree_->set_needs_update_draw_properties();
}
