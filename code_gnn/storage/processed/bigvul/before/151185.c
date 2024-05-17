String GetReferrerPolicy(ReferrerPolicy policy) {
  switch (policy) {
    case kReferrerPolicyAlways:
      return protocol::Network::Request::ReferrerPolicyEnum::UnsafeUrl;
    case kReferrerPolicyDefault:
      if (RuntimeEnabledFeatures::ReducedReferrerGranularityEnabled()) {
        return protocol::Network::Request::ReferrerPolicyEnum::
            StrictOriginWhenCrossOrigin;
      } else {
        return protocol::Network::Request::ReferrerPolicyEnum::
            NoReferrerWhenDowngrade;
      }
    case kReferrerPolicyNoReferrerWhenDowngrade:
      return protocol::Network::Request::ReferrerPolicyEnum::
          NoReferrerWhenDowngrade;
    case kReferrerPolicyNever:
      return protocol::Network::Request::ReferrerPolicyEnum::NoReferrer;
    case kReferrerPolicyOrigin:
      return protocol::Network::Request::ReferrerPolicyEnum::Origin;
    case kReferrerPolicyOriginWhenCrossOrigin:
      return protocol::Network::Request::ReferrerPolicyEnum::
          OriginWhenCrossOrigin;
    case kReferrerPolicySameOrigin:
      return protocol::Network::Request::ReferrerPolicyEnum::SameOrigin;
    case kReferrerPolicyStrictOrigin:
      return protocol::Network::Request::ReferrerPolicyEnum::StrictOrigin;
    case kReferrerPolicyNoReferrerWhenDowngradeOriginWhenCrossOrigin:
      return protocol::Network::Request::ReferrerPolicyEnum::
          StrictOriginWhenCrossOrigin;
  }

  return protocol::Network::Request::ReferrerPolicyEnum::
      NoReferrerWhenDowngrade;
}
