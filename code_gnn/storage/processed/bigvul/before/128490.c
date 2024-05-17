bool ShellSurface::IsResizing() const {
  if (!resizer_)
    return false;

  return resizer_->details().bounds_change &
         ash::WindowResizer::kBoundsChange_Resizes;
}
