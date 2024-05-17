void FrameFetchContext::SetFirstPartyCookieAndRequestorOrigin(
    ResourceRequest& request) {
  if (request.SiteForCookies().IsNull()) {
    if (request.GetFrameType() ==
        network::mojom::RequestContextFrameType::kTopLevel) {
      request.SetSiteForCookies(request.Url());
    } else {
      request.SetSiteForCookies(GetSiteForCookies());
    }
  }

  if (request.GetFrameType() ==
      network::mojom::RequestContextFrameType::kNone) {
    if (!request.RequestorOrigin())
      request.SetRequestorOrigin(GetRequestorOrigin());
  }
}
