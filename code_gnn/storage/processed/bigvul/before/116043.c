std::string GetNewTabBackgroundCSS(const ui::ThemeProvider* theme_provider,
                                   bool bar_attached) {
  int alignment;
  theme_provider->GetDisplayProperty(
      ThemeService::NTP_BACKGROUND_ALIGNMENT, &alignment);

  if (!theme_provider->HasCustomImage(IDR_THEME_NTP_BACKGROUND)) {
    return "-64px";
  }

  if (bar_attached)
    return ThemeService::AlignmentToString(alignment);

#if defined(OS_WIN) || defined(TOOLKIT_VIEWS)
  int offset = browser_defaults::kNewtabBookmarkBarHeight;
#elif defined(OS_MACOSX)
  int offset = bookmarks::kNTPBookmarkBarHeight;
#elif defined(OS_POSIX)
  int offset = BookmarkBarGtk::kBookmarkBarNTPHeight;
#else
  int offset = 0;
#endif

  if (alignment & ThemeService::ALIGN_TOP) {
    if (alignment & ThemeService::ALIGN_LEFT)
      return "0% " + base::IntToString(-offset) + "px";
    else if (alignment & ThemeService::ALIGN_RIGHT)
      return "100% " + base::IntToString(-offset) + "px";
    return "center " + base::IntToString(-offset) + "px";
  }
  return ThemeService::AlignmentToString(alignment);
}
