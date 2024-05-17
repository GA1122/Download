void ChromeContentBrowserClient::NavigationRequestRedirected(
    int frame_tree_node_id,
    const GURL& url,
    net::HttpRequestHeaders* modified_headers) {
  WebContents* web_contents =
      WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  content::BrowserContext* browser_context = web_contents->GetBrowserContext();

  if (!browser_context->IsOffTheRecord()) {
    policy::PolicyHeaderService* policy_header_service =
        policy::PolicyHeaderServiceFactory::GetForBrowserContext(
            browser_context);
    if (policy_header_service)
      policy_header_service->AddPolicyHeaders(url, modified_headers);
  }

  ClientHintsFactory::GetForBrowserContext(browser_context)
      ->GetAdditionalNavigationRequestClientHintsHeaders(url, modified_headers);
}
