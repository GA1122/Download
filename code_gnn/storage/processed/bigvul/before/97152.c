WebFrameLoaderClient::WebFrameLoaderClient(WebFrameImpl* frame)
    : webframe_(frame),
      has_representation_(false),
      sent_initial_response_to_plugin_(false),
      next_navigation_policy_(WebKit::WebNavigationPolicyIgnore) {
}
