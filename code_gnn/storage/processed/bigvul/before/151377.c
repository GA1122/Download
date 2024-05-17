void BaseFetchContext::AddCSPHeaderIfNecessary(Resource::Type type,
                                               ResourceRequest& request) {
  const ContentSecurityPolicy* csp = GetContentSecurityPolicy();
  if (!csp)
    return;
  if (csp->ShouldSendCSPHeader(type))
    request.AddHTTPHeaderField("CSP", "active");
}
