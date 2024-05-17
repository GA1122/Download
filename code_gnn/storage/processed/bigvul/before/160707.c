WebFrameLoadType ReloadFrameLoadTypeFor(
    FrameMsg_Navigate_Type::Value navigation_type) {
  switch (navigation_type) {
    case FrameMsg_Navigate_Type::RELOAD:
    case FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL:
      return WebFrameLoadType::kReload;

    case FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE:
      return WebFrameLoadType::kReloadBypassingCache;

    default:
      NOTREACHED();
      return WebFrameLoadType::kStandard;
  }
}
