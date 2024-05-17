FrameMsg_Navigate_Type::Value GetNavigationType(
    BrowserContext* browser_context,
    const NavigationEntryImpl& entry,
    ReloadType reload_type) {
  switch (reload_type) {
    case ReloadType::NORMAL:
      return FrameMsg_Navigate_Type::RELOAD;
    case ReloadType::MAIN_RESOURCE:
      return FrameMsg_Navigate_Type::RELOAD_MAIN_RESOURCE;
    case ReloadType::BYPASSING_CACHE:
    case ReloadType::DISABLE_LOFI_MODE:
      return FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE;
    case ReloadType::ORIGINAL_REQUEST_URL:
      return FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL;
    case ReloadType::NONE:
      break;   
  }

  if (entry.restore_type() == RestoreType::LAST_SESSION_EXITED_CLEANLY) {
    if (entry.GetHasPostData())
      return FrameMsg_Navigate_Type::RESTORE_WITH_POST;
    return FrameMsg_Navigate_Type::RESTORE;
  }

  return FrameMsg_Navigate_Type::NORMAL;
}
