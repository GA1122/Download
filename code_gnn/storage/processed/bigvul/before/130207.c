bool PrintBackendWin::EnumeratePrinters(PrinterList* printer_list) {
  DCHECK(printer_list);
  DWORD bytes_needed = 0;
  DWORD count_returned = 0;
  const DWORD kLevel = 4;
  BOOL ret = EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL,
                          kLevel, NULL, 0, &bytes_needed, &count_returned);
  if (!bytes_needed)
    return false;
  scoped_ptr<BYTE[]> printer_info_buffer(new BYTE[bytes_needed]);
  ret = EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, kLevel,
                     printer_info_buffer.get(), bytes_needed, &bytes_needed,
                     &count_returned);
  DCHECK(ret);
  if (!ret)
    return false;

  std::string default_printer = GetDefaultPrinterName();
  PRINTER_INFO_4* printer_info =
      reinterpret_cast<PRINTER_INFO_4*>(printer_info_buffer.get());
  for (DWORD index = 0; index < count_returned; index++) {
    ScopedPrinterHandle printer;
    PrinterBasicInfo info;
    if (printer.OpenPrinter(printer_info[index].pPrinterName) &&
        InitBasicPrinterInfo(printer, &info)) {
      info.is_default = (info.printer_name == default_printer);
      printer_list->push_back(info);
    }
  }
  return true;
}
