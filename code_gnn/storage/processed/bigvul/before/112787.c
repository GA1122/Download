void PrintPreviewUI::OnDidGetDefaultPageLayout(
    const PageSizeMargins& page_layout, const gfx::Rect& printable_area,
    bool has_custom_page_size_style) {
  if (page_layout.margin_top < 0 || page_layout.margin_left < 0 ||
      page_layout.margin_bottom < 0 || page_layout.margin_right < 0 ||
      page_layout.content_width < 0 || page_layout.content_height < 0 ||
      printable_area.width() <= 0 || printable_area.height() <= 0) {
    NOTREACHED();
    return;
  }

  base::DictionaryValue layout;
  layout.SetDouble(printing::kSettingMarginTop, page_layout.margin_top);
  layout.SetDouble(printing::kSettingMarginLeft, page_layout.margin_left);
  layout.SetDouble(printing::kSettingMarginBottom, page_layout.margin_bottom);
  layout.SetDouble(printing::kSettingMarginRight, page_layout.margin_right);
  layout.SetDouble(printing::kSettingContentWidth, page_layout.content_width);
  layout.SetDouble(printing::kSettingContentHeight, page_layout.content_height);
  layout.SetInteger(printing::kSettingPrintableAreaX, printable_area.x());
  layout.SetInteger(printing::kSettingPrintableAreaY, printable_area.y());
  layout.SetInteger(printing::kSettingPrintableAreaWidth,
                    printable_area.width());
  layout.SetInteger(printing::kSettingPrintableAreaHeight,
                    printable_area.height());

  base::FundamentalValue has_page_size_style(has_custom_page_size_style);
  web_ui()->CallJavascriptFunction("onDidGetDefaultPageLayout", layout,
                                   has_page_size_style);
}
