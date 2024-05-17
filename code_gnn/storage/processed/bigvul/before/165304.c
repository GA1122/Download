void ChromeContentBrowserClient::NavigationRequestStarted(
    int frame_tree_node_id,
    const GURL& url,
    net::HttpRequestHeaders* extra_headers,
    int* extra_load_flags) {
  WebContents* web_contents =
      WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  content::BrowserContext* browser_context = web_contents->GetBrowserContext();

  ClientHintsFactory::GetForBrowserContext(browser_context)
      ->GetAdditionalNavigationRequestClientHintsHeaders(url, extra_headers);

  prerender::PrerenderContents* prerender_contents =
      prerender::PrerenderContents::FromWebContents(web_contents);
  if (prerender_contents &&
      prerender_contents->prerender_mode() == prerender::PREFETCH_ONLY) {
    *extra_load_flags = net::LOAD_PREFETCH;
    extra_headers->SetHeader(prerender::kPurposeHeaderName,
                             prerender::kPurposeHeaderValue);
  }

  if (!browser_context->IsOffTheRecord()) {
    policy::PolicyHeaderService* policy_header_service =
        policy::PolicyHeaderServiceFactory::GetForBrowserContext(
            browser_context);
    if (policy_header_service)
      policy_header_service->AddPolicyHeaders(url, extra_headers);
  }
}
