static void SetupHistoryPageTest(Browser* browser,
                                 const std::string page_url,
                                 const std::string page_title) {
  HistoryService* history_service =
      browser->profile()->GetHistoryService(Profile::IMPLICIT_ACCESS);
  const GURL history_url = GURL(page_url);
  history_service->AddPage(history_url, history::SOURCE_BROWSED);
  history_service->SetPageTitle(history_url, UTF8ToUTF16(page_title));
}
