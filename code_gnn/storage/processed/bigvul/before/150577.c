net::BackoffEntry::Policy GetBackoffPolicy() {
  net::BackoffEntry::Policy policy = kDefaultBackoffPolicy;
  if (base::FeatureList::IsEnabled(
          features::kDataReductionProxyAggressiveConfigFetch)) {
    policy.num_errors_to_ignore = 2;
    policy.always_use_initial_delay = false;
  }
  return policy;
}
