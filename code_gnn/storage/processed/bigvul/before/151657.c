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

  const std::string value =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kOverscrollHistoryNavigation);
  bool overscroll_enabled = value != "0";
  if (!overscroll_enabled)
    return false;
  if (is_app() || is_devtools() || !is_type_tabbed())
    return false;

  if (value == "1" && bookmark_bar_state_ == BookmarkBar::DETACHED)
    return false;
  return true;
}
