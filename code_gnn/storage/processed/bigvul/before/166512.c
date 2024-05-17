void ChromeContentBrowserClient::NavigationRequestRedirected(
    int frame_tree_node_id,
    const GURL& url,
    base::Optional<net::HttpRequestHeaders>* modified_request_headers) {
  WebContents* web_contents =
      WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  content::BrowserContext* browser_context = web_contents->GetBrowserContext();

  if (!browser_context->IsOffTheRecord()) {
    policy::PolicyHeaderService* policy_header_service =
        policy::PolicyHeaderServiceFactory::GetForBrowserContext(
            browser_context);
    if (policy_header_service) {
      std::unique_ptr<net::HttpRequestHeaders> extra_headers;
      policy_header_service->AddPolicyHeaders(url, &extra_headers);
      if (extra_headers)
        *modified_request_headers = std::move(*extra_headers);
    }
  }
}
