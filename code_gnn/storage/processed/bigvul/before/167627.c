  SiteInstanceTestBrowserClient()
      : privileged_process_id_(-1),
        site_instance_delete_count_(0),
        browsing_instance_delete_count_(0) {
    WebUIControllerFactory::RegisterFactory(
        ContentWebUIControllerFactory::GetInstance());
  }
