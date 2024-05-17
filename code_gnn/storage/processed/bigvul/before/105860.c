void WebPluginAcceleratedSurfaceProxy::StartDrawing() {
  if (!surface_)
    return;

  surface_->MakeCurrent();
  surface_->Clear(gfx::Rect(surface_->GetSize()));
}
