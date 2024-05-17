NavigationPolicy FrameLoader::ShouldContinueForNavigationPolicy(
    const ResourceRequest& request,
    Document* origin_document,
    const SubstituteData& substitute_data,
    DocumentLoader* loader,
    ContentSecurityPolicyDisposition
        should_check_main_world_content_security_policy,
    NavigationType type,
    NavigationPolicy policy,
    FrameLoadType frame_load_type,
    bool is_client_redirect,
    WebTriggeringEventInfo triggering_event_info,
    HTMLFormElement* form) {
  if (request.Url().IsEmpty() || substitute_data.IsValid())
    return kNavigationPolicyCurrentTab;

  if (request.Url().PotentiallyDanglingMarkup() &&
      request.Url().ProtocolIsInHTTPFamily()) {
    Deprecation::CountDeprecation(
        frame_, WebFeature::kCanRequestURLHTTPContainingNewline);
    if (RuntimeEnabledFeatures::RestrictCanRequestURLCharacterSetEnabled())
      return kNavigationPolicyIgnore;
  }

  Settings* settings = frame_->GetSettings();
  if (MaybeCheckCSP(request, type, frame_, policy,
                    should_check_main_world_content_security_policy ==
                        kCheckContentSecurityPolicy,
                    settings && settings->GetBrowserSideNavigationEnabled(),
                    ContentSecurityPolicy::CheckHeaderType::kCheckEnforce) ==
      kNavigationPolicyIgnore) {
    return kNavigationPolicyIgnore;
  }

  bool replaces_current_history_item =
      frame_load_type == kFrameLoadTypeReplaceCurrentItem;
  policy = Client()->DecidePolicyForNavigation(
      request, origin_document, loader, type, policy,
      replaces_current_history_item, is_client_redirect, triggering_event_info,
      form, should_check_main_world_content_security_policy);
  if (policy == kNavigationPolicyCurrentTab ||
      policy == kNavigationPolicyIgnore ||
      policy == kNavigationPolicyHandledByClient ||
      policy == kNavigationPolicyHandledByClientForInitialHistory) {
    return policy;
  } else if (policy == kNavigationPolicyDownload) {
    Client()->DownloadURL(request, String());
  }
  return kNavigationPolicyIgnore;
}
