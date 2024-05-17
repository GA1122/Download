void Textfield::OnNativeThemeChanged(const ui::NativeTheme* theme) {
  gfx::RenderText* render_text = GetRenderText();
  render_text->SetColor(GetTextColor());
  UpdateBackgroundColor();
  render_text->set_selection_color(GetSelectionTextColor());
  render_text->set_selection_background_focused_color(
      GetSelectionBackgroundColor());
  cursor_view_.layer()->SetColor(GetTextColor());
}
