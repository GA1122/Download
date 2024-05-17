gfx::Size ShellSurface::GetPreferredSize() const {
  if (!geometry_.IsEmpty())
    return geometry_.size();

  return surface_ ? surface_->window()->layer()->size() : gfx::Size();
}
