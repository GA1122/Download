void MergeFeaturesFromOriginPolicy(WTF::String& feature_policy,
                                   const String& origin_policy_string) {
  if (origin_policy_string.IsEmpty())
    return;

  std::unique_ptr<OriginPolicy> origin_policy = OriginPolicy::From(
      StringUTF8Adaptor(origin_policy_string).AsStringPiece());
  if (!origin_policy)
    return;

  for (const std::string& policy : origin_policy->GetFeaturePolicies()) {
    if (!feature_policy.IsEmpty()) {
      feature_policy.append(',');
    }
    feature_policy.append(
        WTF::String::FromUTF8(policy.data(), policy.length()));
  }
}
