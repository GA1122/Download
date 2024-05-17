int MockPrinter::GetPrintedPages() const {
  if (printer_status_ != PRINTER_READY)
    return -1;
  return page_number_;
}
