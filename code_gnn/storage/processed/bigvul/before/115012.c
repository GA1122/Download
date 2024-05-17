void TestingAutomationProvider::ResetToDefaultTheme() {
  ThemeServiceFactory::GetForProfile(profile_)->UseDefaultTheme();
}
