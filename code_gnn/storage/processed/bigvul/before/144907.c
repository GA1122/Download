bool RenderWidgetHostViewAura::GetCompositionCharacterBounds(
    uint32_t index,
    gfx::Rect* rect) const {
  DCHECK(rect);

  if (!text_input_manager_ || !text_input_manager_->GetActiveWidget())
    return false;

  const TextInputManager::CompositionRangeInfo* composition_range_info =
      text_input_manager_->GetCompositionRangeInfo();

  if (index >= composition_range_info->character_bounds.size())
    return false;
  *rect = ConvertRectToScreen(composition_range_info->character_bounds[index]);
  return true;
}
