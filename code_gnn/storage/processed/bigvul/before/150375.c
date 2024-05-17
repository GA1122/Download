void ClientControlledShellSurface::OnDragFinished(bool canceled,
                                                  const gfx::Point& location) {
  in_drag_ = false;
  if (drag_finished_callback_)
    drag_finished_callback_.Run(location.x(), location.y(), canceled);
}
