void ClientControlledShellSurface::SetBounds(int64_t display_id,
                                             const gfx::Rect& bounds) {
  TRACE_EVENT2("exo", "ClientControlledShellSurface::SetBounds", "display_id",
               display_id, "bounds", bounds.ToString());

  if (bounds.IsEmpty()) {
    DLOG(WARNING) << "Bounds must be non-empty";
    return;
  }

  SetDisplay(display_id);
  SetGeometry(bounds);
}
