std::string PrintBackendWin::GetDefaultPrinterName() {
  DWORD size = MAX_PATH;
  TCHAR default_printer_name[MAX_PATH];
  if (!::GetDefaultPrinter(default_printer_name, &size))
    return std::string();
  return base::WideToUTF8(default_printer_name);
}
