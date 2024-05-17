void WebUIBidiCheckerBrowserTestRTL::CleanUpOnMainThread() {
  WebUIBidiCheckerBrowserTest::CleanUpOnMainThread();
#if defined(OS_POSIX) && defined(TOOLKIT_GTK)
  gtk_widget_set_default_direction(GTK_TEXT_DIR_LTR);
#endif
  base::i18n::SetICUDefaultLocale(app_locale_);
  ResourceBundle::GetSharedInstance().OverrideLocalePakForTest(FilePath());
  ResourceBundle::GetSharedInstance().ReloadLocaleResources(app_locale_);
}
