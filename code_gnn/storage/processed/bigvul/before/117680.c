Browser* InProcessBrowserTest::CreateBrowserForPopup(Profile* profile) {
  Browser* browser = Browser::CreateForType(Browser::TYPE_POPUP, profile);
  AddBlankTabAndShow(browser);
  return browser;
}
