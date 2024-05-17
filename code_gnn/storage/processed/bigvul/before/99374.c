bool PrintingContext::GetPrinterSettings(HANDLE printer,
                                         const std::wstring& device_name) {
  DCHECK(!in_print_job_);
  scoped_array<uint8> buffer;

  GetPrinterHelper(printer, 9, &buffer);
  if (buffer.get()) {
    PRINTER_INFO_9* info_9 = reinterpret_cast<PRINTER_INFO_9*>(buffer.get());
    if (info_9->pDevMode != NULL) {
      if (!AllocateContext(device_name, info_9->pDevMode)) {
        ResetSettings();
        return false;
      }
      return InitializeSettings(*info_9->pDevMode, device_name, NULL, 0, false);
    }
    buffer.reset();
  }

  GetPrinterHelper(printer, 8, &buffer);
  if (buffer.get()) {
    PRINTER_INFO_8* info_8 = reinterpret_cast<PRINTER_INFO_8*>(buffer.get());
    if (info_8->pDevMode != NULL) {
      if (!AllocateContext(device_name, info_8->pDevMode)) {
        ResetSettings();
        return false;
      }
      return InitializeSettings(*info_8->pDevMode, device_name, NULL, 0, false);
    }
    buffer.reset();
  }

  GetPrinterHelper(printer, 2, &buffer);
  if (buffer.get()) {
    PRINTER_INFO_2* info_2 = reinterpret_cast<PRINTER_INFO_2*>(buffer.get());
    if (info_2->pDevMode != NULL) {
      if (!AllocateContext(device_name, info_2->pDevMode)) {
        ResetSettings();
        return false;
      }
      return InitializeSettings(*info_2->pDevMode, device_name, NULL, 0, false);
    }
    buffer.reset();
  }
  ResetSettings();
  return false;
}
