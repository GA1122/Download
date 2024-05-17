bool RenderWidgetHostViewAura::GetSelectionRange(gfx::Range* range) const {
  range->set_start(selection_range_.start());
  range->set_end(selection_range_.end());
  return true;
}
