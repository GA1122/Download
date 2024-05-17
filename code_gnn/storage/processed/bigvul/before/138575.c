  FakeOneGoogleBarFetcher* one_google_bar_fetcher() {
    return static_cast<FakeOneGoogleBarFetcher*>(
        OneGoogleBarServiceFactory::GetForProfile(browser()->profile())
            ->fetcher_for_testing());
  }
