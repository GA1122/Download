void Textfield::PaintTextAndCursor(gfx::Canvas* canvas) {
  TRACE_EVENT0("views", "Textfield::PaintTextAndCursor");
  canvas->Save();

  gfx::RenderText* render_text = GetRenderText();
  if (text().empty() && !GetPlaceholderText().empty()) {
    int placeholder_text_draw_flags = placeholder_text_draw_flags_;
    if (SkColorGetA(GetBackgroundColor()) != SK_AlphaOPAQUE)
      placeholder_text_draw_flags |= gfx::Canvas::NO_SUBPIXEL_RENDERING;

    canvas->DrawStringRectWithFlags(
        GetPlaceholderText(),
        placeholder_font_list_.has_value() ? placeholder_font_list_.value()
                                           : GetFontList(),
        placeholder_text_color_.value_or(
            ui::MaterialDesignController::IsSecondaryUiMaterial()
                ? SkColorSetA(GetTextColor(), 0x83)
                : kDefaultPlaceholderTextColor),
        render_text->display_rect(), placeholder_text_draw_flags);
  }

  render_text->Draw(canvas);

  if (drop_cursor_visible_) {
    canvas->FillRect(render_text->GetCursorBounds(drop_cursor_position_, true),
                     GetTextColor());
  }

  canvas->Restore();
}
