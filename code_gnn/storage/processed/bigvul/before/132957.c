bool RenderWidgetHostViewAura::GetTextRange(gfx::Range* range) const {
  range->set_start(selection_text_offset_);
  range->set_end(selection_text_offset_ + selection_text_.length());
  return true;
}
