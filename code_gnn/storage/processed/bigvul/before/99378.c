PrintingContext::Result PrintingContext::InitWithSettings(
    const PrintSettings& settings) {
  DCHECK(!in_print_job_);
  settings_ = settings;
  HANDLE printer;
  if (!OpenPrinter(const_cast<wchar_t*>(settings_.device_name().c_str()),
                   &printer,
                   NULL))
    return FAILED;

  Result status = OK;

  if (!GetPrinterSettings(printer, settings_.device_name()))
    status = FAILED;

  ClosePrinter(printer);

  if (status != OK)
    ResetSettings();
  return status;
}
