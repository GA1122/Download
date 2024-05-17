void TestingAutomationProvider::GetBrowserLocale(string16* locale) {
  *locale = ASCIIToUTF16(g_browser_process->GetApplicationLocale());
}
