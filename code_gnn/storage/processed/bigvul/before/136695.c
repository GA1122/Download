void FrameLoader::UpgradeInsecureRequest(ResourceRequest& resource_request,
                                         Document* origin_document) {

  if (!origin_document)
    return;

  if (!(origin_document->GetInsecureRequestPolicy() & kUpgradeInsecureRequests))
    return;

  if (resource_request.GetFrameType() ==
      network::mojom::RequestContextFrameType::kNested) {
    return;
  }

  KURL url = resource_request.Url();
  if (!url.ProtocolIs("http"))
    return;

  if (resource_request.GetFrameType() ==
          network::mojom::RequestContextFrameType::kNone ||
      resource_request.GetRequestContext() ==
          WebURLRequest::kRequestContextForm ||
      (!url.Host().IsNull() &&
       origin_document->InsecureNavigationsToUpgrade()->Contains(
           url.Host().Impl()->GetHash()))) {
    UseCounter::Count(origin_document,
                      WebFeature::kUpgradeInsecureRequestsUpgradedRequest);
    url.SetProtocol("https");
    if (url.Port() == 80)
      url.SetPort(443);
    resource_request.SetURL(url);
  }
}
