ShellSurface::~ShellSurface() {
  DCHECK(!scoped_configure_);
  ash::Shell::GetInstance()->activation_client()->RemoveObserver(this);
  if (surface_) {
    if (scale_ != 1.0)
      surface_->window()->SetTransform(gfx::Transform());
    surface_->SetSurfaceDelegate(nullptr);
    surface_->RemoveSurfaceObserver(this);
  }
  if (parent_)
    parent_->RemoveObserver(this);
  if (resizer_)
    EndDrag(false  );
  if (widget_) {
    ash::wm::GetWindowState(widget_->GetNativeWindow())->RemoveObserver(this);
    widget_->GetNativeWindow()->RemoveObserver(this);
    if (widget_->IsVisible())
      widget_->Hide();
    widget_->CloseNow();
  }
}
