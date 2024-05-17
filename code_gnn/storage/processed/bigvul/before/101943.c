bool MockPrinter::SaveBitmap(
    unsigned int page, const FilePath& filepath) const {
  if (printer_status_ != PRINTER_READY || page >= pages_.size())
    return false;

  pages_[page]->image().SaveToPng(filepath);
  return true;
}
