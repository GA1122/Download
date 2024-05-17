void Browser::OpenAutofillHelpTabAndActivate() {
  GURL help_url = google_util::AppendGoogleLocaleParam(GURL(kAutofillHelpUrl));
  AddSelectedTabWithURL(help_url, PageTransition::LINK);
}
