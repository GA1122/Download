void ShellSurface::OnSurfaceDestroying(Surface* surface) {
  if (resizer_)
    EndDrag(false  );
  if (widget_)
    SetMainSurface(widget_->GetNativeWindow(), nullptr);
  surface->RemoveSurfaceObserver(this);
  surface_ = nullptr;

  if (widget_)
    widget_->Hide();

  if (!surface_destroyed_callback_.is_null())
    surface_destroyed_callback_.Run();
}
