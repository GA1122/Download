void OutOfProcessInstance::OnGeometryChanged(double old_zoom,
                                             float old_device_scale) {
  if (zoom_ != old_zoom || device_scale_ != old_device_scale)
    engine_->ZoomUpdated(zoom_ * device_scale_);

  available_area_ = pp::Rect(plugin_size_);
  int doc_width = GetDocumentPixelWidth();
  if (doc_width < available_area_.width()) {
    available_area_.Offset((available_area_.width() - doc_width) / 2, 0);
    available_area_.set_width(doc_width);
  }
  int bottom_of_document =
      GetDocumentPixelHeight() + (top_toolbar_height_ * device_scale_);
  if (bottom_of_document < available_area_.height())
    available_area_.set_height(bottom_of_document);

  CalculateBackgroundParts();
  engine_->PageOffsetUpdated(available_area_.point());
  engine_->PluginSizeUpdated(available_area_.size());

  if (!document_size_.GetArea())
    return;
  paint_manager_.InvalidateRect(pp::Rect(pp::Point(), plugin_size_));
}
