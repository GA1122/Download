void WebUIBidiCheckerBrowserTestRTL::SetUpOnMainThread() {
  WebUIBidiCheckerBrowserTest::SetUpOnMainThread();
  FilePath pak_path;
  app_locale_ = base::i18n::GetConfiguredLocale();
  ASSERT_TRUE(PathService::Get(base::FILE_MODULE, &pak_path));
  pak_path = pak_path.DirName();
  pak_path = pak_path.AppendASCII("pseudo_locales");
  pak_path = pak_path.AppendASCII("fake-bidi");
  pak_path = pak_path.ReplaceExtension(FILE_PATH_LITERAL("pak"));
  ResourceBundle::GetSharedInstance().OverrideLocalePakForTest(pak_path);
  ResourceBundle::GetSharedInstance().ReloadLocaleResources("he");
  base::i18n::SetICUDefaultLocale("he");
#if defined(OS_POSIX) && defined(TOOLKIT_GTK)
  gtk_widget_set_default_direction(GTK_TEXT_DIR_RTL);
#endif
}
