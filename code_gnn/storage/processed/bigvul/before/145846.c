std::unique_ptr<base::DictionaryValue> ParsePrintSettings(
    int command_id,
    const base::DictionaryValue* params,
    HeadlessPrintSettings* settings) {
  params->GetBoolean("landscape", &settings->landscape);
  params->GetBoolean("displayHeaderFooter", &settings->display_header_footer);
  params->GetBoolean("printBackground", &settings->should_print_backgrounds);
  params->GetDouble("scale", &settings->scale);
  if (settings->scale > kScaleMaxVal / 100 ||
      settings->scale < kScaleMinVal / 100)
    return CreateInvalidParamResponse(command_id, "scale");
  params->GetString("pageRanges", &settings->page_ranges);
  params->GetBoolean("ignoreInvalidPageRanges",
                     &settings->ignore_invalid_page_ranges);

  double paper_width_in_inch = printing::kLetterWidthInch;
  double paper_height_in_inch = printing::kLetterHeightInch;
  params->GetDouble("paperWidth", &paper_width_in_inch);
  params->GetDouble("paperHeight", &paper_height_in_inch);
  if (paper_width_in_inch <= 0)
    return CreateInvalidParamResponse(command_id, "paperWidth");
  if (paper_height_in_inch <= 0)
    return CreateInvalidParamResponse(command_id, "paperHeight");
  settings->paper_size_in_points =
      gfx::Size(paper_width_in_inch * printing::kPointsPerInch,
                paper_height_in_inch * printing::kPointsPerInch);

  double default_margin_in_inch = 1000.0 / printing::kHundrethsMMPerInch;
  double margin_top_in_inch = default_margin_in_inch;
  double margin_bottom_in_inch = default_margin_in_inch;
  double margin_left_in_inch = default_margin_in_inch;
  double margin_right_in_inch = default_margin_in_inch;
  params->GetDouble("marginTop", &margin_top_in_inch);
  params->GetDouble("marginBottom", &margin_bottom_in_inch);
  params->GetDouble("marginLeft", &margin_left_in_inch);
  params->GetDouble("marginRight", &margin_right_in_inch);
  if (margin_top_in_inch < 0)
    return CreateInvalidParamResponse(command_id, "marginTop");
  if (margin_bottom_in_inch < 0)
    return CreateInvalidParamResponse(command_id, "marginBottom");
  if (margin_left_in_inch < 0)
    return CreateInvalidParamResponse(command_id, "marginLeft");
  if (margin_right_in_inch < 0)
    return CreateInvalidParamResponse(command_id, "marginRight");
  settings->margins_in_points.top =
      margin_top_in_inch * printing::kPointsPerInch;
  settings->margins_in_points.bottom =
      margin_bottom_in_inch * printing::kPointsPerInch;
  settings->margins_in_points.left =
      margin_left_in_inch * printing::kPointsPerInch;
  settings->margins_in_points.right =
      margin_right_in_inch * printing::kPointsPerInch;

  return nullptr;
}
