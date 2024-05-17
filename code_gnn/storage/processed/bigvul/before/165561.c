static String StripURLForUseInReport(
    ExecutionContext* context,
    const KURL& url,
    RedirectStatus redirect_status,
    const ContentSecurityPolicy::DirectiveType& effective_type) {
  if (!url.IsValid())
    return String();
  if (!url.IsHierarchical() || url.ProtocolIs("file"))
    return url.Protocol();

  bool can_safely_expose_url =
      context->GetSecurityOrigin()->CanRequest(url) ||
      (redirect_status == RedirectStatus::kNoRedirect &&
       effective_type != ContentSecurityPolicy::DirectiveType::kFrameSrc &&
       effective_type != ContentSecurityPolicy::DirectiveType::kObjectSrc);

  if (can_safely_expose_url) {
    if (url.ProtocolIsInHTTPFamily())
      return url.StrippedForUseAsReferrer();
  }
  return SecurityOrigin::Create(url)->ToString();
}
