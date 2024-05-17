void WebFrameLoaderClient::dispatchDecidePolicyForNewWindowAction(
    WebCore::FramePolicyFunction function,
    const WebCore::NavigationAction& action,
    const WebCore::ResourceRequest& request,
    PassRefPtr<WebCore::FormState> form_state,
    const WebCore::String& frame_name) {
  WebNavigationPolicy navigation_policy;
  if (!ActionSpecifiesNavigationPolicy(action, &navigation_policy))
    navigation_policy = WebKit::WebNavigationPolicyNewForegroundTab;

  PolicyAction policy_action;
  if (navigation_policy == WebKit::WebNavigationPolicyDownload) {
    policy_action = PolicyDownload;
  } else {
    policy_action = PolicyUse;

    next_navigation_policy_ = navigation_policy;
  }
  (webframe_->frame()->loader()->*function)(policy_action);
}
