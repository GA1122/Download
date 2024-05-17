void LayerTreeHostImpl::SetFullViewportDamage() {
  SetViewportDamage(gfx::Rect(DrawViewportSize()));
}
