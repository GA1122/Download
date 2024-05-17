void PrintHeaderFooterByRenderText(
    const string16& text,
    WebKit::WebCanvas* canvas,
    HeaderFooterPaint paint,
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    printing::HorizontalHeaderFooterPosition horizontal_position,
    printing::VerticalHeaderFooterPosition vertical_position,
    double offset_to_baseline) {
  scoped_ptr<gfx::RenderText> render_text(gfx::RenderText::CreateRenderText());
  render_text->SetText(text);
  int font_size = printing::kSettingHeaderFooterFontSize / webkit_scale_factor;
  render_text->SetFontSize(font_size);
  gfx::Size text_size = render_text->GetStringSize();
  int text_height = text_size.height();
  SkScalar margin_left = page_layout.margin_left / webkit_scale_factor;
  SkScalar margin_top = page_layout.margin_top / webkit_scale_factor;
  SkScalar content_height = page_layout.content_height / webkit_scale_factor;

  int text_width = text_size.width();
  SkPoint point = GetHeaderFooterPosition(webkit_scale_factor, page_layout,
                                          horizontal_position,
                                          vertical_position, offset_to_baseline,
                                          SkScalarToDouble(text_width));
  point.set(point.x() + margin_left, point.y() + margin_top);

  gfx::Rect rect(point.x(), point.y(), text_width, text_height);
  if (vertical_position == printing::TOP) {
    const int content_top = margin_top + 1;
    if (rect.bottom() < content_top)
      rect.set_y(content_top - rect.height());
  } else {   
    const int content_bottom = margin_top + content_height - 1;
    if (rect.y() > content_bottom)
      rect.set_y(content_bottom);
  }
  render_text->SetDisplayRect(rect);

  int save_count = canvas->save();
  canvas->translate(-margin_left, -margin_top);
  {
    gfx::Canvas gfx_canvas(canvas);
    render_text->Draw(&gfx_canvas);
  }
  canvas->restoreToCount(save_count);
}
