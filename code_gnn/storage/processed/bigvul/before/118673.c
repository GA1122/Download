Browser* InProcessBrowserTest::CreateBrowserForPopup(Profile* profile) {
  Browser* browser =
      new Browser(Browser::CreateParams(Browser::TYPE_POPUP, profile,
                  chrome::GetActiveDesktop()));
  AddBlankTabAndShow(browser);
  return browser;
}
