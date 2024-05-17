bool PrintBackendWin::IsValidPrinter(const std::string& printer_name) {
  ScopedPrinterHandle printer_handle;
  return printer_handle.OpenPrinter(base::UTF8ToWide(printer_name).c_str());
}
