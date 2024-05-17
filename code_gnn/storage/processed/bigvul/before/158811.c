bool Browser::CanOverscrollContent() const {
#if defined(OS_WIN)
  bool allow_overscroll = ui::GetTouchScreensAvailability() ==
      ui::TouchScreensAvailability::ENABLED;
#elif defined(USE_AURA)
  bool allow_overscroll = true;
#else
  bool allow_overscroll = false;
#endif

  if (!allow_overscroll)
    return false;

  if (is_app() || is_devtools() || !is_type_tabbed())
    return false;

  return content::OverscrollConfig::GetHistoryNavigationMode() !=
         content::OverscrollConfig::HistoryNavigationMode::kDisabled;
}
