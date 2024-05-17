bool ChildProcessSecurityPolicyImpl::IsWebSafeScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);

  return base::ContainsKey(schemes_okay_to_request_in_any_process_, scheme);
}
