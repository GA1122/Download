std::string PrintBackendWin::GetPrinterDriverInfo(
    const std::string& printer_name) {
  ScopedPrinterHandle printer;
  if (!printer.OpenPrinter(base::UTF8ToWide(printer_name).c_str())) {
    return std::string();
  }
  return GetDriverInfo(printer);
}
