void WebFrameLoaderClient::dispatchDecidePolicyForNavigationAction(
    WebCore::FramePolicyFunction function,
    const WebCore::NavigationAction& action,
    const WebCore::ResourceRequest& request,
    PassRefPtr<WebCore::FormState> form_state) {
  PolicyAction policy_action = PolicyIgnore;

  if (webframe_->client() && !request.url().isNull()) {
    WebNavigationPolicy navigation_policy =
        WebKit::WebNavigationPolicyCurrentTab;
    ActionSpecifiesNavigationPolicy(action, &navigation_policy);

    const WebDataSourceImpl* ds = webframe_->GetProvisionalDataSourceImpl();
    if (ds) {
      GURL url = ds->request().url();
      if (url.SchemeIs(webkit_glue::kBackForwardNavigationScheme)) {
        HandleBackForwardNavigation(url);
        navigation_policy = WebKit::WebNavigationPolicyIgnore;
      } else {
        bool is_redirect = ds->hasRedirectChain();

        WebNavigationType webnav_type =
            WebDataSourceImpl::toWebNavigationType(action.type());

        navigation_policy = webframe_->client()->decidePolicyForNavigation(
            webframe_, ds->request(), webnav_type, navigation_policy,
            is_redirect);
      }
    }

    if (navigation_policy == WebKit::WebNavigationPolicyCurrentTab) {
      policy_action = PolicyUse;
    } else if (navigation_policy == WebKit::WebNavigationPolicyDownload) {
      policy_action = PolicyDownload;
    } else {
      if (navigation_policy != WebKit::WebNavigationPolicyIgnore) {
        WrappedResourceRequest webreq(request);
        webframe_->client()->loadURLExternally(
            webframe_, webreq, navigation_policy);
      }
      policy_action = PolicyIgnore;
    }
  }

  (webframe_->frame()->loader()->*function)(policy_action);
}
