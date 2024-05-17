WebNavigationPolicy RenderFrameImpl::DecidePolicyForNavigation(
    RenderFrame* render_frame,
    const NavigationPolicyInfo& info) {
  Referrer referrer(RenderViewImpl::GetReferrerFromRequest(info.frame,
                                                           info.urlRequest));

  if (SiteIsolationPolicy::IsSwappedOutStateForbidden() && is_subframe_) {
  } else {
    if (is_swapped_out_) {
      if (info.urlRequest.url() != GURL(kSwappedOutURL)) {
        if (info.frame->parent() == NULL) {
          OpenURL(info.frame, info.urlRequest.url(), referrer,
                  info.defaultPolicy);
          return blink::WebNavigationPolicyIgnore;   
        }

        return blink::WebNavigationPolicyIgnore;
      }

      return info.defaultPolicy;
    }
  }

  const GURL& url = info.urlRequest.url();

  DocumentState* document_state = static_cast<DocumentState*>(info.extraData);
  bool is_content_initiated =
      document_state->navigation_state()->IsContentInitiated();

  if (is_content_initiated) {
    bool is_form_post =
        ((info.navigationType == blink::WebNavigationTypeFormSubmitted) ||
            (info.navigationType == blink::WebNavigationTypeFormResubmitted)) &&
        base::EqualsASCII(base::StringPiece16(info.urlRequest.httpMethod()),
                          "POST");
    bool browser_handles_request =
        render_view_->renderer_preferences_
            .browser_handles_non_local_top_level_requests
        && IsNonLocalTopLevelNavigation(url, info.frame, info.navigationType,
                                        is_form_post);
    if (!browser_handles_request) {
      browser_handles_request = IsTopLevelNavigation(info.frame) &&
          render_view_->renderer_preferences_
              .browser_handles_all_top_level_requests;
    }

    if (browser_handles_request) {
      OpenURL(info.frame, url, referrer, info.defaultPolicy);
      return blink::WebNavigationPolicyIgnore;   
    }
  }

  GURL old_url(info.frame->dataSource()->request().url());

  if (!info.frame->parent() && is_content_initiated &&
      !url.SchemeIs(url::kAboutScheme)) {
    bool send_referrer = false;

    int cumulative_bindings = RenderProcess::current()->GetEnabledBindings();
    bool is_initial_navigation = render_view_->history_list_length_ == 0;
    bool should_fork = HasWebUIScheme(url) || HasWebUIScheme(old_url) ||
        (cumulative_bindings & BINDINGS_POLICY_WEB_UI) ||
        url.SchemeIs(kViewSourceScheme) ||
        (info.frame->isViewSourceModeEnabled() &&
            info.navigationType != blink::WebNavigationTypeReload);

    if (!should_fork && url.SchemeIs(url::kFileScheme)) {
      GURL source_url(old_url);
      if (is_initial_navigation && source_url.is_empty() &&
          info.frame->opener())
        source_url = info.frame->opener()->top()->document().url();
      DCHECK(!source_url.is_empty());
      should_fork = !source_url.SchemeIs(url::kFileScheme);
    }

    if (!should_fork) {
      should_fork = GetContentClient()->renderer()->ShouldFork(
          info.frame, url, info.urlRequest.httpMethod().utf8(),
          is_initial_navigation, info.isRedirect, &send_referrer);
    }

    if (should_fork) {
      OpenURL(info.frame, url, send_referrer ? referrer : Referrer(),
              info.defaultPolicy);
      return blink::WebNavigationPolicyIgnore;   
    }
  }

  bool is_fork =
      old_url == GURL(url::kAboutBlankURL) &&
      render_view_->historyBackListCount() < 1 &&
      render_view_->historyForwardListCount() < 1 &&
      info.frame->opener() == NULL &&
      info.frame->parent() == NULL &&
      is_content_initiated &&
      info.defaultPolicy == blink::WebNavigationPolicyCurrentTab &&
      info.navigationType == blink::WebNavigationTypeOther;

  if (is_fork) {
    OpenURL(info.frame, url, Referrer(), info.defaultPolicy);
    return blink::WebNavigationPolicyIgnore;
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableBrowserSideNavigation) &&
      info.urlRequest.checkForBrowserSideNavigation() &&
      ShouldMakeNetworkRequestForURL(url)) {
    BeginNavigation(&info.urlRequest);
    return blink::WebNavigationPolicyIgnore;
  }

  return info.defaultPolicy;
}
