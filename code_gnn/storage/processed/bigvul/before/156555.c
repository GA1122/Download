void ChildProcessSecurityPolicyImpl::RegisterPseudoScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);
  DCHECK_EQ(0U, pseudo_schemes_.count(scheme)) << "Add schemes at most once.";
  DCHECK_EQ(0U, schemes_okay_to_request_in_any_process_.count(scheme))
      << "Pseudo implies not web-safe.";
  DCHECK_EQ(0U, schemes_okay_to_commit_in_any_process_.count(scheme))
      << "Pseudo implies not web-safe.";

  pseudo_schemes_.insert(scheme);
}
