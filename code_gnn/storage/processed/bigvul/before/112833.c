void PrintHeaderFooterText(
    const string16& text,
    WebKit::WebCanvas* canvas,
    HeaderFooterPaint paint,
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    printing::HorizontalHeaderFooterPosition horizontal_position,
    printing::VerticalHeaderFooterPosition vertical_position,
    double offset_to_baseline) {
#if defined(USE_RENDER_TEXT)
  paint->SetText(text);
  paint->SetFontSize(printing::kSettingHeaderFooterFontSize);
  double text_width_in_points = paint->GetStringSize().width();
  SkPoint point = GetHeaderFooterPosition(webkit_scale_factor, page_layout,
                                          horizontal_position,
                                          vertical_position, offset_to_baseline,
                                          text_width_in_points);
  gfx::FontList font_list(
      gfx::Font(printing::kSettingHeaderFooterFontFamilyName,
                printing::kSettingHeaderFooterFontSize / webkit_scale_factor));
  paint->SetFontList(font_list);
  gfx::Size size(paint->GetStringSize());
  gfx::Rect rect(point.x(), point.y() - paint->GetBaseline(),
                 size.width(), size.height());
  paint->SetDisplayRect(rect);
  {
    SkMatrix m = canvas->getTotalMatrix();
    ui::ScaleFactor device_scale_factor = ui::GetScaleFactorFromScale(
        SkScalarAbs(m.getScaleX()));
    scoped_ptr<gfx::Canvas> gfx_canvas(gfx::Canvas::CreateCanvasWithoutScaling(
        canvas, device_scale_factor));
    paint->Draw(gfx_canvas.get());
  }
#else
  size_t text_byte_length = text.length() * sizeof(char16);
  double text_width_in_points = SkScalarToDouble(paint.measureText(
      text.c_str(), text_byte_length));
  SkPoint point = GetHeaderFooterPosition(webkit_scale_factor, page_layout,
                                          horizontal_position,
                                          vertical_position, offset_to_baseline,
                                          text_width_in_points);
  paint.setTextSize(SkDoubleToScalar(
      paint.getTextSize() / webkit_scale_factor));
  canvas->drawText(text.c_str(), text_byte_length, point.x(), point.y(),
                   paint);
#endif
}
