void OutOfProcessInstance::DidChangeView(const pp::View& view) {
  pp::Rect view_rect(view.GetRect());
  float old_device_scale = device_scale_;
  float device_scale = view.GetDeviceScale();
  pp::Size view_device_size(view_rect.width() * device_scale,
                            view_rect.height() * device_scale);

  if (view_device_size != plugin_size_ || device_scale != device_scale_) {
    device_scale_ = device_scale;
    plugin_dip_size_ = view_rect.size();
    plugin_size_ = view_device_size;

    paint_manager_.SetSize(view_device_size, device_scale_);

    pp::Size new_image_data_size = PaintManager::GetNewContextSize(
        image_data_.size(),
        plugin_size_);
    if (new_image_data_size != image_data_.size()) {
      image_data_ = pp::ImageData(this,
                                  PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                                  new_image_data_size,
                                  false);
      first_paint_ = true;
    }

    if (image_data_.is_null()) {
      DCHECK(plugin_size_.IsEmpty());
      return;
    }

    OnGeometryChanged(zoom_, old_device_scale);
  }

  if (!stop_scrolling_) {
    pp::Point scroll_offset(view.GetScrollOffset());
    pp::FloatPoint scroll_offset_float(scroll_offset.x(),
                                       scroll_offset.y() - top_toolbar_height_);
    scroll_offset_float = BoundScrollOffsetToDocument(scroll_offset_float);
    engine_->ScrolledToXPosition(scroll_offset_float.x() * device_scale_);
    engine_->ScrolledToYPosition(scroll_offset_float.y() * device_scale_);
  }
}
