void Instance::DocumentSizeUpdated(const pp::Size& size) {
  document_size_ = size;

  OnGeometryChanged(zoom_, device_scale_);
}
