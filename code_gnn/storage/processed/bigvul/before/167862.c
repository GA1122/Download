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
    HTMLFormElement* form,
    mojom::blink::BlobURLTokenPtr blob_url_token,
    bool check_with_client) {
  if (request.Url().IsEmpty() || substitute_data.IsValid())
    return kNavigationPolicyCurrentTab;

  if (request.Url().PotentiallyDanglingMarkup() &&
      request.Url().ProtocolIsInHTTPFamily()) {
    Deprecation::CountDeprecation(
        frame_, WebFeature::kCanRequestURLHTTPContainingNewline);
    if (RuntimeEnabledFeatures::RestrictCanRequestURLCharacterSetEnabled())
      return kNavigationPolicyIgnore;
  }

  if (MaybeCheckCSP(request, type, frame_, policy,
                    should_check_main_world_content_security_policy ==
                        kCheckContentSecurityPolicy,
                    ContentSecurityPolicy::CheckHeaderType::kCheckEnforce) ==
      kNavigationPolicyIgnore) {
    return kNavigationPolicyIgnore;
  }

  bool replaces_current_history_item =
      frame_load_type == kFrameLoadTypeReplaceCurrentItem;
  policy = Client()->DecidePolicyForNavigation(
      request, origin_document, loader, type, policy,
      replaces_current_history_item, is_client_redirect, triggering_event_info,
      form, should_check_main_world_content_security_policy,
      std::move(blob_url_token));
  if (!check_with_client)
    CHECK_EQ(kNavigationPolicyCurrentTab, policy);
  DCHECK(policy == kNavigationPolicyCurrentTab ||
         policy == kNavigationPolicyIgnore ||
         policy == kNavigationPolicyHandledByClient ||
         policy == kNavigationPolicyHandledByClientForInitialHistory)
      << policy;
  return policy;
}
