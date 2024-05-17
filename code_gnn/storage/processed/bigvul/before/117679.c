Browser* InProcessBrowserTest::CreateBrowser(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  AddBlankTabAndShow(browser);
  return browser;
}
