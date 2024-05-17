  static void Set(ContentMainDelegate* delegate, bool single_process) {
    ContentClient* content_client = oxide::ContentClient::GetInstance();
    content_client->browser_ = delegate->CreateContentBrowserClient();

    if (single_process) {
      content_client->renderer_ = delegate->CreateContentRendererClient();
      content_client->utility_ = delegate->CreateContentUtilityClient();
    }
  }
