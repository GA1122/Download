void ClientControlledShellSurface::SetCanMaximize(bool can_maximize) {
  TRACE_EVENT1("exo", "ClientControlledShellSurface::SetCanMaximize",
               "can_maximzie", can_maximize);
  can_maximize_ = can_maximize;
  if (widget_)
    widget_->OnSizeConstraintsChanged();
}
