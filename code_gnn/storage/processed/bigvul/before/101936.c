const MockPrinterPage* MockPrinter::GetPrintedPage(unsigned int pageno) const {
  if (pages_.size() > pageno)
    return pages_[pageno];
  else
    return NULL;
}
