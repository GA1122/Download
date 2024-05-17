void AddAdditionalRequestHeaders(
    net::HttpRequestHeaders* headers,
    std::unique_ptr<net::HttpRequestHeaders> embedder_additional_headers,
    const GURL& url,
    FrameMsg_Navigate_Type::Value navigation_type,
    BrowserContext* browser_context,
    const std::string& method,
    const std::string user_agent_override,
    bool has_user_gesture,
    base::Optional<url::Origin> initiator_origin,
    FrameTreeNode* frame_tree_node) {
  if (!url.SchemeIsHTTPOrHTTPS())
    return;

  if (!base::GetFieldTrialParamByFeatureAsBool(features::kDataSaverHoldback,
                                               "holdback_web", false)) {
    bool is_reload =
        navigation_type == FrameMsg_Navigate_Type::RELOAD ||
        navigation_type == FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE ||
        navigation_type == FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL;
    if (is_reload)
      headers->RemoveHeader("Save-Data");

    if (GetContentClient()->browser()->IsDataSaverEnabled(browser_context))
      headers->SetHeaderIfMissing("Save-Data", "on");
  }

  if (embedder_additional_headers)
    headers->MergeFrom(*(embedder_additional_headers.get()));

  headers->SetHeaderIfMissing("Upgrade-Insecure-Requests", "1");

  headers->SetHeaderIfMissing(net::HttpRequestHeaders::kUserAgent,
                              user_agent_override.empty()
                                  ? GetContentClient()->GetUserAgent()
                                  : user_agent_override);

  if (IsSecMetadataEnabled()) {
    std::string site_value = "cross-site";
    if (initiator_origin) {
      url::Origin target_origin = url::Origin::Create(url);
      if (initiator_origin->IsSameOriginWith(target_origin)) {
        site_value = "same-origin";
      } else if (net::registry_controlled_domains::SameDomainOrHost(
                     *initiator_origin, target_origin,
                     net::registry_controlled_domains::
                         INCLUDE_PRIVATE_REGISTRIES)) {
        site_value = "same-site";
      }
    }
    std::string value = base::StringPrintf(
        "cause=\"%s\", destination=\"%s\", site=\"%s\"",
        has_user_gesture ? "user-activated" : "forced",
        frame_tree_node->IsMainFrame() ? "document" : "nested-document",
        site_value.c_str());
    headers->SetHeaderIfMissing("Sec-Metadata", value);
  }

  std::string origin_policy_request;
  if (OriginPolicyThrottle::ShouldRequestOriginPolicy(url,
                                                      &origin_policy_request)) {
    headers->SetHeader(net::HttpRequestHeaders::kSecOriginPolicy,
                       origin_policy_request);
  }

  if (!NeedsHTTPOrigin(headers, method))
    return;

  url::Origin origin;
  if (frame_tree_node->IsMainFrame()) {
    origin = url::Origin::Create(url);
  } else if ((frame_tree_node->active_sandbox_flags() &
              blink::WebSandboxFlags::kOrigin) ==
             blink::WebSandboxFlags::kNone) {
    origin = frame_tree_node->frame_tree()->root()->current_origin();
  }

  headers->SetHeader(net::HttpRequestHeaders::kOrigin, origin.Serialize());
}
