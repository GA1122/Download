void ShellSurface::WindowClosing() {
  if (resizer_)
    EndDrag(true  );
  SetEnabled(false);
  widget_ = nullptr;
}
