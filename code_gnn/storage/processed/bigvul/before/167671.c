ReferrerPolicy CSSStyleSheetResource::GetReferrerPolicy() const {
  ReferrerPolicy referrer_policy = kReferrerPolicyDefault;
  String referrer_policy_header =
      GetResponse().HttpHeaderField(http_names::kReferrerPolicy);
  if (!referrer_policy_header.IsNull()) {
    SecurityPolicy::ReferrerPolicyFromHeaderValue(
        referrer_policy_header, kDoNotSupportReferrerPolicyLegacyKeywords,
        &referrer_policy);
  }
  return referrer_policy;
}
