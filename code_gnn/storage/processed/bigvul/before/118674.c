Browser* InProcessBrowserTest::CreateIncognitoBrowser() {
  Browser* incognito = new Browser(
      Browser::CreateParams(browser()->profile()->GetOffTheRecordProfile(),
                            chrome::GetActiveDesktop()));
  AddBlankTabAndShow(incognito);
  return incognito;
}
