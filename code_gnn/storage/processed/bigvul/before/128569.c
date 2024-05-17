pp::Var Instance::GetLinkAtPosition(const pp::Point& point) {
  pp::Point offset_point(point);
  ScalePoint(device_scale_, &offset_point);
  offset_point.set_x(offset_point.x() - available_area_.x());
  return engine_->GetLinkAtPosition(offset_point);
}
