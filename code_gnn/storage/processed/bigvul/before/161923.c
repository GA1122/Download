bool IsPrintToPdfRequested(const base::DictionaryValue& job_settings) {
  bool print_to_pdf = false;
  if (!job_settings.GetBoolean(kSettingPrintToPDF, &print_to_pdf))
    NOTREACHED();
  return print_to_pdf;
}
