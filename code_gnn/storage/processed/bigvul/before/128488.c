void ShellSurface::GetWidgetHitTestMask(gfx::Path* mask) const {
  DCHECK(WidgetHasHitTestMask());
  surface_->GetHitTestMask(mask);
  gfx::Point origin = surface_->window()->bounds().origin();
  mask->offset(SkIntToScalar(origin.x()), SkIntToScalar(origin.y()));
}
