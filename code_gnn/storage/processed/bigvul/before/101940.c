MockPrinterPage::MockPrinterPage(const void* source_data,
                                 uint32 source_size,
                                 const printing::Image& image)
    : source_size_(source_size),
      image_(image) {
  source_data_.reset(new uint8[source_size]);
  if (source_data_.get())
    memcpy(source_data_.get(), source_data, source_size);
}
