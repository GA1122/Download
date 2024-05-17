  ~SiteInstanceTestBrowserClient() {
    WebUIControllerFactory::UnregisterFactoryForTesting(&factory_);
  }
