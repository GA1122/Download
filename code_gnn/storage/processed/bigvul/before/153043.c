void PDFiumEngine::GetRegion(const pp::Point& location,
                             pp::ImageData* image_data,
                             void** region,
                             int* stride) const {
  if (image_data->is_null()) {
    DCHECK(plugin_size_.IsEmpty());
    *stride = 0;
    *region = nullptr;
    return;
  }
  char* buffer = static_cast<char*>(image_data->data());
  *stride = image_data->stride();

  pp::Point offset_location = location + page_offset_;
  if (!buffer ||
      !pp::Rect(page_offset_, plugin_size_).Contains(offset_location)) {
    *region = nullptr;
    return;
  }

  buffer += location.y() * (*stride);
  buffer += (location.x() + page_offset_.x()) * 4;
  *region = buffer;
}
