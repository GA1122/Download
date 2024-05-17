void ClientControlledShellSurface::OnDragStarted(int component) {
  in_drag_ = true;
  if (drag_started_callback_)
    drag_started_callback_.Run(component);
}
