void OutOfProcessInstance::Scroll(const pp::Point& point) {
  if (!image_data_.is_null())
    paint_manager_.ScrollRect(available_area_, point);
}
