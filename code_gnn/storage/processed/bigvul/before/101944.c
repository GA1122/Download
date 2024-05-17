bool MockPrinter::SaveSource(
    unsigned int page, const FilePath& filepath) const {
  if (printer_status_ != PRINTER_READY || page >= pages_.size())
    return false;
  const uint8* source_data = pages_[page]->source_data();
  uint32 source_size = pages_[page]->source_size();
  file_util::WriteFile(filepath, reinterpret_cast<const char*>(source_data),
                       source_size);
  return true;
}
