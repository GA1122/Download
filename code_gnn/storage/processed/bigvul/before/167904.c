void Document::setDomain(const String& raw_domain,
                         ExceptionState& exception_state) {
  UseCounter::Count(*this, WebFeature::kDocumentSetDomain);

  if (IsSandboxed(kSandboxDocumentDomain)) {
    exception_state.ThrowSecurityError(
        "Assignment is forbidden for sandboxed iframes.");
    return;
  }

  if (SchemeRegistry::IsDomainRelaxationForbiddenForURLScheme(
          GetSecurityOrigin()->Protocol())) {
    exception_state.ThrowSecurityError("Assignment is forbidden for the '" +
                                       GetSecurityOrigin()->Protocol() +
                                       "' scheme.");
    return;
  }

  bool success = false;
  String new_domain = SecurityOrigin::CanonicalizeHost(raw_domain, &success);
  if (!success) {
    exception_state.ThrowSecurityError("'" + raw_domain +
                                       "' could not be parsed properly.");
    return;
  }

  if (new_domain.IsEmpty()) {
    exception_state.ThrowSecurityError("'" + new_domain +
                                       "' is an empty domain.");
    return;
  }

  if (!RuntimeEnabledFeatures::NullableDocumentDomainEnabled() ||
      new_domain != "null") {
    OriginAccessEntry access_entry(GetSecurityOrigin()->Protocol(), new_domain,
                                   OriginAccessEntry::kAllowSubdomains);
    OriginAccessEntry::MatchResult result =
        access_entry.MatchesOrigin(*GetSecurityOrigin());
    if (result == OriginAccessEntry::kDoesNotMatchOrigin) {
      exception_state.ThrowSecurityError(
          "'" + new_domain + "' is not a suffix of '" + domain() + "'.");
      return;
    }

    if (result == OriginAccessEntry::kMatchesOriginButIsPublicSuffix) {
      exception_state.ThrowSecurityError("'" + new_domain +
                                         "' is a top-level domain.");
      return;
    }
  }

  if (frame_) {
    UseCounter::Count(*this,
                      GetSecurityOrigin()->Port() == 0
                          ? WebFeature::kDocumentDomainSetWithDefaultPort
                          : WebFeature::kDocumentDomainSetWithNonDefaultPort);
    bool was_cross_domain = frame_->IsCrossOriginSubframe();
    GetMutableSecurityOrigin()->SetDomainFromDOM(new_domain);
    if (View() && (was_cross_domain != frame_->IsCrossOriginSubframe()))
      View()->CrossOriginStatusChanged();

    frame_->GetScriptController().UpdateSecurityOrigin(GetSecurityOrigin());
  }
}
