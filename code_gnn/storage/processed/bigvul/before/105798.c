void Label::PaintText(gfx::Canvas* canvas,
                      const std::wstring& text,
                      const gfx::Rect& text_bounds,
                      int flags) {
  canvas->DrawStringInt(WideToUTF16Hack(text), font_, color_,
                        text_bounds.x(), text_bounds.y(),
                        text_bounds.width(), text_bounds.height(), flags);

  if (HasFocus() || paint_as_focused_) {
    gfx::Rect focus_bounds = text_bounds;
    focus_bounds.Inset(-kFocusBorderPadding, -kFocusBorderPadding);
    canvas->DrawFocusRect(focus_bounds.x(), focus_bounds.y(),
                          focus_bounds.width(), focus_bounds.height());
  }
}
