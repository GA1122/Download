void AddAdditionalRequestHeaders(
    net::HttpRequestHeaders* headers,
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

  headers->SetHeaderIfMissing("Upgrade-Insecure-Requests", "1");

  headers->SetHeaderIfMissing(
      net::HttpRequestHeaders::kUserAgent,
      user_agent_override.empty()
          ? GetContentClient()->browser()->GetUserAgent()
          : user_agent_override);

  if (IsSecMetadataEnabled() && IsOriginSecure(url)) {
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
    std::string destination;
    std::string mode = "navigate";
    switch (frame_tree_node->frame_owner_element_type()) {
      case blink::FrameOwnerElementType::kNone:
        destination = "document";
        break;
      case blink::FrameOwnerElementType::kObject:
        destination = "object";
        mode = "no-cors";
        break;
      case blink::FrameOwnerElementType::kEmbed:
        destination = "embed";
        mode = "no-cors";
        break;
      case blink::FrameOwnerElementType::kIframe:
      case blink::FrameOwnerElementType::kFrame:
      case blink::FrameOwnerElementType::kPortal:
        destination = "nested-document";
    }
    headers->SetHeaderIfMissing("Sec-Fetch-Dest", destination.c_str());
    headers->SetHeaderIfMissing("Sec-Fetch-Mode", mode.c_str());
    headers->SetHeaderIfMissing("Sec-Fetch-Site", site_value.c_str());
    headers->SetHeaderIfMissing("Sec-Fetch-User",
                                has_user_gesture ? "?T" : "?F");
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
