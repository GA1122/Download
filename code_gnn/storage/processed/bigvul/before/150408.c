void ClientControlledShellSurface::StartDrag(int component,
                                             const gfx::Point& location) {
  TRACE_EVENT2("exo", "ClientControlledShellSurface::StartDrag", "component",
               component, "location", location.ToString());

  if (!widget_)
    return;
  AttemptToStartDrag(component, location);
}
