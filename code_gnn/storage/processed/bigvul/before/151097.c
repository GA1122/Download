content::WebContents* DevToolsToolboxDelegate::OpenURLFromTab(
    content::WebContents* source,
    const content::OpenURLParams& params) {
  DCHECK(source == web_contents());
  if (!params.url.SchemeIs(content::kChromeDevToolsScheme))
    return NULL;
  content::NavigationController::LoadURLParams load_url_params(params.url);
  source->GetController().LoadURLWithParams(load_url_params);
  return source;
}
