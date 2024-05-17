const KURL Document::SiteForCookies() const {

  if (IsHTMLImport())
    return ImportsController()->Master()->SiteForCookies();

  if (!GetFrame())
    return SecurityOrigin::UrlWithUniqueSecurityOrigin();

  Frame& top = GetFrame()->Tree().Top();
  KURL top_document_url;
  if (top.IsLocalFrame()) {
    top_document_url = ToLocalFrame(top).GetDocument()->Url();
  } else {
    SecurityOrigin* origin = top.GetSecurityContext()->GetSecurityOrigin();
    if (origin)
      top_document_url = KURL(NullURL(), origin->ToString());
    else
      top_document_url = SecurityOrigin::UrlWithUniqueSecurityOrigin();
  }

  if (SchemeRegistry::ShouldTreatURLSchemeAsFirstPartyWhenTopLevel(
          top_document_url.Protocol()))
    return top_document_url;

  const OriginAccessEntry& access_entry =
      top.IsLocalFrame()
          ? ToLocalFrame(top).GetDocument()->AccessEntryFromURL()
          : OriginAccessEntry(top_document_url.Protocol(),
                              top_document_url.Host(),
                              OriginAccessEntry::kAllowRegisterableDomains);
  const Frame* current_frame = GetFrame();
  while (current_frame) {
    while (current_frame->IsLocalFrame() &&
           ToLocalFrame(current_frame)->GetDocument()->IsSrcdocDocument())
      current_frame = current_frame->Tree().Parent();
    DCHECK(current_frame);

    if (access_entry.MatchesDomain(
            *current_frame->GetSecurityContext()->GetSecurityOrigin()) ==
        OriginAccessEntry::kDoesNotMatchOrigin)
      return SecurityOrigin::UrlWithUniqueSecurityOrigin();

    current_frame = current_frame->Tree().Parent();
  }

  return top_document_url;
}
