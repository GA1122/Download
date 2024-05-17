bool RenderWidgetHostViewAura::GetCompositionCharacterBounds(
    uint32 index,
    gfx::Rect* rect) const {
  DCHECK(rect);
  if (index >= composition_character_bounds_.size())
    return false;
  *rect = ConvertRectToScreen(composition_character_bounds_[index]);
  return true;
}
