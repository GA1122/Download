void PPB_Widget_Impl::SetLocation(const PP_Rect* location) {
  location_ = *location;
  SetLocationInternal(location);
 }
