void FrameLoader::UpgradeInsecureRequest(ResourceRequest& resource_request,
                                         ExecutionContext* origin_context) {

  if (!origin_context)
    return;

  if (!(origin_context->GetSecurityContext().GetInsecureRequestPolicy() &
        kUpgradeInsecureRequests)) {
    mojom::RequestContextType context = resource_request.GetRequestContext();
    if (context != mojom::RequestContextType::UNSPECIFIED &&
        resource_request.Url().ProtocolIs("http") &&
        !origin_context->GetSecurityContext().GetMixedAutoUpgradeOptOut() &&
        MixedContentChecker::ShouldAutoupgrade(
            origin_context->GetHttpsState(),
            WebMixedContent::ContextTypeFromRequestContext(context, false))) {
      if (origin_context->IsDocument()) {
        Document* document = static_cast<Document*>(origin_context);
        document->AddConsoleMessage(
            MixedContentChecker::CreateConsoleMessageAboutFetchAutoupgrade(
                origin_context->Url(), resource_request.Url()));
        resource_request.SetUkmSourceId(document->UkmSourceID());
      }
      resource_request.SetIsAutomaticUpgrade(true);
    } else {
      return;
    }
  }

  if (resource_request.GetFrameType() ==
      network::mojom::RequestContextFrameType::kNested) {
    return;
  }

  resource_request.SetUpgradeIfInsecure(true);

  KURL url = resource_request.Url();

  if (!url.ProtocolIs("http") ||
      SecurityOrigin::Create(url)->IsPotentiallyTrustworthy()) {
    return;
  }

  if (resource_request.GetFrameType() ==
          network::mojom::RequestContextFrameType::kNone ||
      resource_request.GetRequestContext() == mojom::RequestContextType::FORM ||
      (!url.Host().IsNull() && origin_context->GetSecurityContext()
                                   .InsecureNavigationsToUpgrade()
                                   ->Contains(url.Host().Impl()->GetHash()))) {
    UseCounter::Count(origin_context,
                      WebFeature::kUpgradeInsecureRequestsUpgradedRequest);
    url.SetProtocol("https");
    if (url.Port() == 80)
      url.SetPort(443);
    resource_request.SetURL(url);
  }
}
