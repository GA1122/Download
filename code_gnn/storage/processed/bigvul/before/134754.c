void GuestViewBase::SetUpSizing(const base::DictionaryValue& params) {
  bool auto_size_enabled = auto_size_enabled_;
  params.GetBoolean(guestview::kAttributeAutoSize, &auto_size_enabled);

  int max_height = max_auto_size_.height();
  int max_width = max_auto_size_.width();
  params.GetInteger(guestview::kAttributeMaxHeight, &max_height);
  params.GetInteger(guestview::kAttributeMaxWidth, &max_width);

  int min_height = min_auto_size_.height();
  int min_width = min_auto_size_.width();
  params.GetInteger(guestview::kAttributeMinHeight, &min_height);
  params.GetInteger(guestview::kAttributeMinWidth, &min_width);

  double element_height = 0.0;
  double element_width = 0.0;
  params.GetDouble(guestview::kElementHeight, &element_height);
  params.GetDouble(guestview::kElementWidth, &element_width);

  int normal_height = normal_size_.height();
  int normal_width = normal_size_.width();
  bool element_size_is_logical = false;
  params.GetBoolean(guestview::kElementSizeIsLogical, &element_size_is_logical);
  if (element_size_is_logical) {
    normal_height = LogicalPixelsToPhysicalPixels(element_height);
    normal_width = LogicalPixelsToPhysicalPixels(element_width);
  } else {
    normal_height = lround(element_height);
    normal_width = lround(element_width);
  }

  SetSizeParams set_size_params;
  set_size_params.enable_auto_size.reset(new bool(auto_size_enabled));
  set_size_params.min_size.reset(new gfx::Size(min_width, min_height));
  set_size_params.max_size.reset(new gfx::Size(max_width, max_height));
  set_size_params.normal_size.reset(new gfx::Size(normal_width, normal_height));

  SetSize(set_size_params);
}
