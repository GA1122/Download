bool PrintWebViewHelper::IsPrintToPdfRequested(
    const DictionaryValue& job_settings) {
  bool print_to_pdf = false;
  if (!job_settings.GetBoolean(printing::kSettingPrintToPDF, &print_to_pdf))
    NOTREACHED();
  return print_to_pdf;
}
