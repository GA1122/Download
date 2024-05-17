void ExternalProtocolHandler::LaunchUrlWithoutSecurityCheck(
    const GURL& url,
    content::WebContents* web_contents) {
  if (!web_contents)
    return;

  platform_util::OpenExternal(
      Profile::FromBrowserContext(web_contents->GetBrowserContext()), url);
}
