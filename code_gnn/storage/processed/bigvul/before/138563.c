  static std::unique_ptr<KeyedService> CreateOneGoogleBarService(
      content::BrowserContext* context) {
    GaiaCookieManagerService* cookie_service =
        GaiaCookieManagerServiceFactory::GetForProfile(
            Profile::FromBrowserContext(context));
    return base::MakeUnique<OneGoogleBarService>(
        cookie_service, base::MakeUnique<FakeOneGoogleBarFetcher>());
  }
