bool ContentSecurityPolicy::allowPluginTypeForDocument(
    const Document& document,
    const String& type,
    const String& typeAttribute,
    const KURL& url,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (document.contentSecurityPolicy() &&
      !document.contentSecurityPolicy()->allowPluginType(type, typeAttribute,
                                                         url))
    return false;

  LocalFrame* frame = document.frame();
  if (frame && frame->tree().parent() && document.isPluginDocument()) {
    ContentSecurityPolicy* parentCSP =
        frame->tree().parent()->securityContext()->contentSecurityPolicy();
    if (parentCSP && !parentCSP->allowPluginType(type, typeAttribute, url))
      return false;
  }

  return true;
}
