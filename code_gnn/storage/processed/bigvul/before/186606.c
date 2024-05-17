 void UrlData::set_has_opaque_data(bool has_opaque_data) {
  if (has_opaque_data_)
// void UrlData::set_is_cors_cross_origin(bool is_cors_cross_origin) {
//   if (is_cors_cross_origin_)
      return;
  has_opaque_data_ = has_opaque_data;
//   is_cors_cross_origin_ = is_cors_cross_origin;
  }