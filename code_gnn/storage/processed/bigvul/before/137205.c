const gfx::Range& Textfield::GetSelectedRange() const {
  return GetRenderText()->selection();
}
