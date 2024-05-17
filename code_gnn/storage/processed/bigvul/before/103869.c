WebNavigationPolicy RenderView::decidePolicyForNavigation(
    WebFrame* frame, const WebURLRequest& request, WebNavigationType type,
    const WebNode&, WebNavigationPolicy default_policy, bool is_redirect) {
  if (is_swapped_out_) {
    DCHECK(request.url() == GURL("about:swappedout"));
    return default_policy;
  }

  const GURL& url = request.url();

  bool is_content_initiated =
      NavigationState::FromDataSource(frame->provisionalDataSource())->
          is_content_initiated();

  if (is_content_initiated &&
      renderer_preferences_.browser_handles_top_level_requests &&
      IsNonLocalTopLevelNavigation(url, frame, type)) {
    GURL referrer(request.httpHeaderField(WebString::fromUTF8("Referer")));
    page_id_ = -1;
    last_page_id_sent_to_browser_ = -1;
    OpenURL(url, referrer, default_policy);
    return WebKit::WebNavigationPolicyIgnore;   
  }

  GURL old_url(frame->document().url());

  if (!frame->parent() && (is_content_initiated || is_redirect) &&
      default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      request.httpMethod() == "GET" && !url.SchemeIs(chrome::kAboutScheme)) {
    bool send_referrer = false;
    bool should_fork =
        BindingsPolicy::is_web_ui_enabled(enabled_bindings_) ||
        frame->isViewSourceModeEnabled() ||
        url.SchemeIs(chrome::kViewSourceScheme);

    if (!should_fork) {
      should_fork = content::GetContentClient()->renderer()->ShouldFork(
          frame, url, is_content_initiated, &send_referrer);
    }

    if (should_fork) {
      GURL referrer(request.httpHeaderField(WebString::fromUTF8("Referer")));
      OpenURL(url, send_referrer ? referrer : GURL(), default_policy);
      return WebKit::WebNavigationPolicyIgnore;   
    }
  }

  bool is_fork =
      old_url == GURL(chrome::kAboutBlankURL) &&
      historyBackListCount() < 1 &&
      historyForwardListCount() < 1 &&
      frame->opener() == NULL &&
      frame->parent() == NULL &&
      is_content_initiated &&
      default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      type == WebKit::WebNavigationTypeOther;

  bool is_noreferrer_and_blank_target =
      frame->parent() == NULL &&
      frame->document().url().isEmpty() &&
      historyBackListCount() < 1 &&
      historyForwardListCount() < 1 &&
      request.httpHeaderField(WebString::fromUTF8("Referer")).isNull() &&
      opener_suppressed_ &&
      frame->opener() == NULL &&
      frame->name().isNull() &&
      !creator_url_.is_empty() &&
      is_content_initiated &&
      default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      type == WebKit::WebNavigationTypeOther;

  if (is_fork || is_noreferrer_and_blank_target) {
    OpenURL(url, GURL(), default_policy);
    return WebKit::WebNavigationPolicyIgnore;
  }

  return default_policy;
}
