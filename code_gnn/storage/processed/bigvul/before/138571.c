  void OnWillCreateBrowserContextServices(content::BrowserContext* context) {
    OneGoogleBarServiceFactory::GetInstance()->SetTestingFactory(
        context, &LocalNTPOneGoogleBarSmokeTest::CreateOneGoogleBarService);
  }
