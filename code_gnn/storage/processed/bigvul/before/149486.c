static String stripURLForUseInReport(
    ExecutionContext* context,
    const KURL& url,
    RedirectStatus redirectStatus,
    const ContentSecurityPolicy::DirectiveType& effectiveType) {
  if (!url.isValid())
    return String();
  if (!url.isHierarchical() || url.protocolIs("file"))
    return url.protocol();

  bool canSafelyExposeURL =
      context->getSecurityOrigin()->canRequest(url) ||
      (redirectStatus == RedirectStatus::NoRedirect &&
       effectiveType != ContentSecurityPolicy::DirectiveType::FrameSrc &&
       effectiveType != ContentSecurityPolicy::DirectiveType::ObjectSrc);

  if (canSafelyExposeURL) {
    if (url.protocolIsInHTTPFamily())
      return url.strippedForUseAsReferrer();
  }
  return SecurityOrigin::create(url)->toString();
}
