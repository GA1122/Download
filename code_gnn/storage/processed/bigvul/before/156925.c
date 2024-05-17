void AddAdditionalRequestHeaders(
    net::HttpRequestHeaders* headers,
    std::unique_ptr<net::HttpRequestHeaders> embedder_additional_headers,
    const GURL& url,
    FrameMsg_Navigate_Type::Value navigation_type,
    BrowserContext* browser_context,
    const std::string& method,
    const std::string user_agent_override,
    FrameTreeNode* frame_tree_node) {
  if (!url.SchemeIsHTTPOrHTTPS())
    return;

  if (!base::FeatureList::IsEnabled(features::kDataSaverHoldback)) {
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
