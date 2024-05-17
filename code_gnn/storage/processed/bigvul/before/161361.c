String referrerPolicy(net::URLRequest::ReferrerPolicy referrer_policy) {
  switch (referrer_policy) {
    case net::URLRequest::CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE:
      return Network::Request::ReferrerPolicyEnum::NoReferrerWhenDowngrade;
    case net::URLRequest::
        REDUCE_REFERRER_GRANULARITY_ON_TRANSITION_CROSS_ORIGIN:
      return Network::Request::ReferrerPolicyEnum::StrictOriginWhenCrossOrigin;
    case net::URLRequest::ORIGIN_ONLY_ON_TRANSITION_CROSS_ORIGIN:
      return Network::Request::ReferrerPolicyEnum::OriginWhenCrossOrigin;
    case net::URLRequest::NEVER_CLEAR_REFERRER:
      return Network::Request::ReferrerPolicyEnum::Origin;
    case net::URLRequest::ORIGIN:
      return Network::Request::ReferrerPolicyEnum::Origin;
    case net::URLRequest::NO_REFERRER:
      return Network::Request::ReferrerPolicyEnum::NoReferrer;
    default:
      break;
  }
  NOTREACHED();
  return Network::Request::ReferrerPolicyEnum::NoReferrerWhenDowngrade;
}
