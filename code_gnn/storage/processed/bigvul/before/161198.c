  std::vector<GURL> GetRedirects(const GURL& url) {
    history::HistoryService* history_service =
        HistoryServiceFactory::GetForProfile(
            browser()->profile(), ServiceAccessType::EXPLICIT_ACCESS);

    std::vector<GURL> rv;
    history_service->QueryRedirectsFrom(
        url,
        base::Bind(&RedirectTest::OnRedirectQueryComplete,
                   base::Unretained(this),
                   &rv),
        &tracker_);
    content::RunMessageLoop();
    return rv;
  }
