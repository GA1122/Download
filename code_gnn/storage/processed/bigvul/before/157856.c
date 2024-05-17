bool WebContentsImpl::SetDeviceEmulationSize(const gfx::Size& new_size) {
  device_emulation_size_ = new_size;
  RenderWidgetHostView* rwhv = GetMainFrame()->GetView();

  const gfx::Size current_size = rwhv->GetViewBounds().size();
  if (view_size_before_emulation_.IsEmpty())
    view_size_before_emulation_ = current_size;

  if (current_size != new_size)
    rwhv->SetSize(new_size);

  return current_size != new_size;
}
