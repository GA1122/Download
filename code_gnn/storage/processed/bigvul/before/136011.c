ChildProcessSecurityPolicyImpl* ChildProcessSecurityPolicyImpl::GetInstance() {
  return base::Singleton<ChildProcessSecurityPolicyImpl>::get();
}
