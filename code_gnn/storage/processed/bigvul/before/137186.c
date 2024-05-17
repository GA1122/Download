bool Textfield::GetCompositionCharacterBounds(uint32_t index,
                                              gfx::Rect* rect) const {
  DCHECK(rect);
  if (!HasCompositionText())
    return false;
  gfx::Range composition_range;
  model_->GetCompositionTextRange(&composition_range);
  DCHECK(!composition_range.is_empty());

  size_t text_index = composition_range.start() + index;
  if (composition_range.end() <= text_index)
    return false;
  gfx::RenderText* render_text = GetRenderText();
  if (!render_text->IsValidCursorIndex(text_index)) {
    text_index =
        render_text->IndexOfAdjacentGrapheme(text_index, gfx::CURSOR_BACKWARD);
  }
  if (text_index < composition_range.start())
    return false;
  const gfx::SelectionModel caret(text_index, gfx::CURSOR_BACKWARD);
  *rect = render_text->GetCursorBounds(caret, false);
  ConvertRectToScreen(this, rect);
  return true;
}
