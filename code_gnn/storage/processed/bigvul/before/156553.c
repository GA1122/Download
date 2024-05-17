bool ChildProcessSecurityPolicyImpl::IsPseudoScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);

  return base::ContainsKey(pseudo_schemes_, scheme);
}
