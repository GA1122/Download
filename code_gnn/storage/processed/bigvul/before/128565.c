int Instance::GetDocumentPixelWidth() const {
  return static_cast<int>(ceil(document_size_.width() * zoom_ * device_scale_));
}
