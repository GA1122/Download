void LaunchURL(
    const GURL& url,
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    ui::PageTransition page_transition,
    bool has_user_gesture) {
  content::WebContents* web_contents = web_contents_getter.Run();
  if (!web_contents)
    return;

  prerender::PrerenderContents* prerender_contents =
      prerender::PrerenderContents::FromWebContents(web_contents);
  if (prerender_contents) {
    prerender_contents->Destroy(prerender::FINAL_STATUS_UNSUPPORTED_SCHEME);
    prerender::ReportPrerenderExternalURL();
    return;
  }

  bool is_whitelisted = false;
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  PolicyBlacklistService* service =
      PolicyBlacklistFactory::GetForBrowserContext(profile);
  if (service) {
    const policy::URLBlacklist::URLBlacklistState url_state =
        service->GetURLBlacklistState(url);
    is_whitelisted =
        url_state == policy::URLBlacklist::URLBlacklistState::URL_IN_WHITELIST;
  }

  if (is_whitelisted) {
    ExternalProtocolHandler::LaunchUrlWithoutSecurityCheck(url, web_contents);
  } else {
    ExternalProtocolHandler::LaunchUrl(
        url, web_contents->GetRenderViewHost()->GetProcess()->GetID(),
        web_contents->GetRenderViewHost()->GetRoutingID(), page_transition,
        has_user_gesture);
  }
}
