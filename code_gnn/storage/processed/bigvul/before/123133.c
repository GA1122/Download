void RenderWidgetHostViewAndroid::SetSize(const gfx::Size& size) {
  if (surface_texture_transport_.get())
    surface_texture_transport_->SetSize(size);

  host_->WasResized();
}
