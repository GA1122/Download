void Instance::DrawText(const pp::Point& top_center, PP_ResourceString id) {
  std::string str(GetLocalizedString(id));

  pp::FontDescription_Dev description;
  description.set_family(PP_FONTFAMILY_SANSSERIF);
  description.set_size(kMessageTextSize * device_scale_);
  pp::Font_Dev font(this, description);
  int length = font.MeasureSimpleText(str);
  pp::Point point(top_center);
  point.set_x(point.x() - length / 2);
  DCHECK(!image_data_.is_null());
  font.DrawSimpleText(&image_data_, str, point, kMessageTextColor);
}
