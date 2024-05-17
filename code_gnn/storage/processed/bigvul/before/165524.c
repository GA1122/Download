Vector<CSPHeaderAndType> ContentSecurityPolicy::Headers() const {
  Vector<CSPHeaderAndType> headers;
  headers.ReserveInitialCapacity(policies_.size());
  for (const auto& policy : policies_) {
    headers.UncheckedAppend(
        CSPHeaderAndType(policy->Header(), policy->HeaderType()));
  }
  return headers;
}
