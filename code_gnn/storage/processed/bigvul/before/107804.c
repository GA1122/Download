void Browser::OpenHelpTab() {
  GURL help_url(kHelpContentUrl);
#if defined(OS_CHROMEOS)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kGuestSession))
    help_url = GURL(kHelpContentOnlineUrl);
#endif
  GURL localized_help_url = google_util::AppendGoogleLocaleParam(help_url);
  AddSelectedTabWithURL(localized_help_url, PageTransition::AUTO_BOOKMARK);
}
