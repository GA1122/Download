void PrintWebViewHelper::UpdateFrameMarginsCssInfo(
    const DictionaryValue& settings) {
  int margins_type = 0;
  if (!settings.GetInteger(printing::kSettingMarginsType, &margins_type))
    margins_type = printing::DEFAULT_MARGINS;
  ignore_css_margins_ = (margins_type != printing::DEFAULT_MARGINS);
}
