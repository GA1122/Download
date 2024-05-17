void WorkerFetchContext::SetFirstPartyCookieAndRequestorOrigin(
    ResourceRequest& out_request) {
  if (out_request.SiteForCookies().IsNull())
    out_request.SetSiteForCookies(GetSiteForCookies());
  if (!out_request.RequestorOrigin())
    out_request.SetRequestorOrigin(GetSecurityOrigin());
}
