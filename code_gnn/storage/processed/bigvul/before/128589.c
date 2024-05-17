void Instance::InvalidateWidget(pp::Widget_Dev widget,
                                const pp::Rect& dirty_rect) {
  if (v_scrollbar_.get() && *v_scrollbar_ == widget) {
    if (!image_data_.is_null())
      v_scrollbar_->Paint(dirty_rect.pp_rect(), &image_data_);
  } else if (h_scrollbar_.get() && *h_scrollbar_ == widget) {
    if (!image_data_.is_null())
      h_scrollbar_->Paint(dirty_rect.pp_rect(), &image_data_);
  } else {
    return;
  }

  pp::Rect dirty_rect_scaled = dirty_rect;
  ScaleRect(device_scale_, &dirty_rect_scaled);
  paint_manager_.InvalidateRect(dirty_rect_scaled);
}
