void SetCustomMarginsIfSelected(const DictionaryValue& job_settings,
                                PrintMsg_PrintPages_Params* settings) {
  bool default_margins_selected;
  if (!job_settings.GetBoolean(printing::kSettingDefaultMarginsSelected,
                               &default_margins_selected)) {
    NOTREACHED();
    default_margins_selected = true;
  }

  if (default_margins_selected)
    return;

  DictionaryValue* custom_margins;
  if (!job_settings.GetDictionary(printing::kSettingMargins,
                                  &custom_margins)) {
    NOTREACHED();
    return;
  }

  double custom_margin_top_in_points = 0;
  double custom_margin_left_in_points = 0;
  double custom_margin_right_in_points = 0;
  double custom_margin_bottom_in_points = 0;
  if (!custom_margins->GetDouble(printing::kSettingMarginTop,
                                 &custom_margin_top_in_points) ||
      !custom_margins->GetDouble(printing::kSettingMarginLeft,
                                 &custom_margin_left_in_points) ||
      !custom_margins->GetDouble(printing::kSettingMarginRight,
                                 &custom_margin_right_in_points) ||
      !custom_margins->GetDouble(printing::kSettingMarginBottom,
                                 &custom_margin_bottom_in_points)) {
    NOTREACHED();
    return;
  }

  int dpi = GetDPI(&settings->params);
  double custom_margin_top_in_dots = ConvertUnitDouble(
      custom_margin_top_in_points, printing::kPointsPerInch, dpi);
  double custom_margin_left_in_dots = ConvertUnitDouble(
      custom_margin_left_in_points, printing::kPointsPerInch, dpi);
  double custom_margin_right_in_dots = ConvertUnitDouble(
      custom_margin_right_in_points, printing::kPointsPerInch, dpi);
  double custom_margin_bottom_in_dots = ConvertUnitDouble(
      custom_margin_bottom_in_points, printing::kPointsPerInch, dpi);


  if (custom_margin_left_in_dots < 0 || custom_margin_right_in_dots < 0 ||
      custom_margin_top_in_dots < 0 || custom_margin_bottom_in_dots < 0) {
    NOTREACHED();
    return;
  }

  if (settings->params.page_size.width() < custom_margin_left_in_dots +
          custom_margin_right_in_dots ||
      settings->params.page_size.height() < custom_margin_top_in_dots +
          custom_margin_bottom_in_dots) {
    NOTREACHED();
    return;
  }

  settings->params.margin_top = custom_margin_top_in_dots;
  settings->params.margin_left = custom_margin_left_in_dots;
  settings->params.printable_size.set_width(
      settings->params.page_size.width() - custom_margin_right_in_dots -
          custom_margin_left_in_dots);
  settings->params.printable_size.set_height(
      settings->params.page_size.height() - custom_margin_bottom_in_dots -
          custom_margin_top_in_dots);
}
