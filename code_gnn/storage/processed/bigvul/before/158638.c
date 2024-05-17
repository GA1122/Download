bool FrameFetchContext::ShouldBlockFetchAsCredentialedSubresource(
    const ResourceRequest& resource_request,
    const KURL& url) const {
  if (resource_request.GetFrameType() !=
      network::mojom::RequestContextFrameType::kNone) {
    return false;
  }

  if (url.User().IsEmpty() && url.Pass().IsEmpty())
    return false;

  if (resource_request.GetRequestContext() ==
      WebURLRequest::kRequestContextXMLHttpRequest) {
    return false;
  }

  if (Url().User() == url.User() && Url().Pass() == url.Pass() &&
      SecurityOrigin::Create(url)->IsSameSchemeHostPort(GetSecurityOrigin())) {
    return false;
  }

  CountDeprecation(WebFeature::kRequestedSubresourceWithEmbeddedCredentials);

  return RuntimeEnabledFeatures::BlockCredentialedSubresourcesEnabled();
}
