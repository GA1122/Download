bool IsReload(FrameMsg_Navigate_Type::Value navigation_type) {
  return navigation_type == FrameMsg_Navigate_Type::RELOAD ||
         navigation_type == FrameMsg_Navigate_Type::RELOAD_IGNORING_CACHE ||
         navigation_type == FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL;
}
