bool ChromeContentRendererClient::ShouldOverridePageVisibilityState(
    const content::RenderView* render_view,
    WebKit::WebPageVisibilityState* override_state) const {
  if (!prerender::PrerenderHelper::IsPrerendering(render_view))
    return false;

  *override_state = WebKit::WebPageVisibilityStatePrerender;
  return true;
}
