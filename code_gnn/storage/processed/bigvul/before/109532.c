void PrintWebViewHelper::PrintHeaderAndFooter(
    WebKit::WebCanvas* canvas,
    int page_number,
    int total_pages,
    float webkit_scale_factor,
    const printing::PageSizeMargins& page_layout,
    const DictionaryValue& header_footer_info,
    const PrintMsg_Print_Params& params) {
  skia::VectorPlatformDeviceSkia* device =
      static_cast<skia::VectorPlatformDeviceSkia*>(canvas->getTopDevice());
  device->setDrawingArea(SkPDFDevice::kMargin_DrawingArea);

#if defined(USE_RENDER_TEXT)
  scoped_ptr<gfx::RenderText> render_text(gfx::RenderText::CreateInstance());
  render_text->set_clip_to_display_rect(false);
  gfx::FontList font_list(
      gfx::Font(printing::kSettingHeaderFooterFontFamilyName,
                printing::kSettingHeaderFooterFontSize));
  gfx::RenderText* paint = render_text.get();
#else
  SkPaint paint;
  paint.setColor(SK_ColorBLACK);
  paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
  paint.setTextSize(SkDoubleToScalar(printing::kSettingHeaderFooterFontSize));
  paint.setTypeface(SkTypeface::CreateFromName(
      printing::kSettingHeaderFooterFontFamilyName, SkTypeface::kNormal));
#endif

  string16 date;
  string16 title;
  if (!header_footer_info.GetString(printing::kSettingHeaderFooterTitle,
                                    &title) ||
      !header_footer_info.GetString(printing::kSettingHeaderFooterDate,
                                    &date)) {
    NOTREACHED();
  }
  string16 header_text = date + title;

  SkRect header_vertical_bounds;
#if defined(USE_RENDER_TEXT)
  paint->SetFontList(font_list);
  paint->SetText(header_text);
  {
    gfx::Rect rect(gfx::Point(), paint->GetStringSize());
    header_vertical_bounds = gfx::RectToSkRect(rect);
    header_vertical_bounds.offset(0, -render_text->GetBaseline());
  }
#else
  paint.measureText(header_text.c_str(), header_text.length() * sizeof(char16),
                    &header_vertical_bounds, 0);
#endif

  double text_height = printing::kSettingHeaderFooterInterstice +
                       header_vertical_bounds.height();
  if (text_height <= page_layout.margin_top) {
    PrintHeaderFooterText(date, canvas, paint, webkit_scale_factor, page_layout,
                          printing::LEFT, printing::TOP,
                          header_vertical_bounds.top());
    PrintHeaderFooterText(title, canvas, paint, webkit_scale_factor,
                          page_layout, printing::CENTER, printing::TOP,
                          header_vertical_bounds.top());
  }

  string16 page_of_total_pages = base::IntToString16(page_number) +
                                 UTF8ToUTF16("/") +
                                 base::IntToString16(total_pages);
  string16 url;
  if (!header_footer_info.GetString(printing::kSettingHeaderFooterURL, &url)) {
    NOTREACHED();
  }
  string16 footer_text = page_of_total_pages + url;

  SkRect footer_vertical_bounds;
#if defined(USE_RENDER_TEXT)
  paint->SetFontList(font_list);
  paint->SetText(footer_text);
  {
    gfx::Rect rect(gfx::Point(), paint->GetStringSize());
    footer_vertical_bounds = gfx::RectToSkRect(rect);
    footer_vertical_bounds.offset(0, -paint->GetBaseline());
  }
#else
  paint.measureText(footer_text.c_str(), footer_text.length() * sizeof(char16),
                    &footer_vertical_bounds, 0);
#endif

  text_height = printing::kSettingHeaderFooterInterstice +
                footer_vertical_bounds.height();
  if (text_height <= page_layout.margin_bottom) {
    PrintHeaderFooterText(page_of_total_pages, canvas, paint,
                          webkit_scale_factor, page_layout, printing::RIGHT,
                          printing::BOTTOM, footer_vertical_bounds.bottom());
    PrintHeaderFooterText(url, canvas, paint, webkit_scale_factor, page_layout,
                          printing::LEFT, printing::BOTTOM,
                          footer_vertical_bounds.bottom());
  }

  device->setDrawingArea(SkPDFDevice::kContent_DrawingArea);
}
