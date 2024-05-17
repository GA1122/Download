Browser* InProcessBrowserTest::CreateIncognitoBrowser() {
  Browser* incognito =
      Browser::Create(browser()->profile()->GetOffTheRecordProfile());
  AddBlankTabAndShow(incognito);
  return incognito;
}
