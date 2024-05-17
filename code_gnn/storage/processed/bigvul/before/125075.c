  SiteInstanceTestBrowserClient()
      : privileged_process_id_(-1) {
    WebUIControllerFactory::RegisterFactory(&factory_);
  }
