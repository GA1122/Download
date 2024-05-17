std::string SupervisedUserService::GetDebugPolicyProviderName() const {
#if DCHECK_IS_ON()
  return "Supervised User Service";
#else
  IMMEDIATE_CRASH();
#endif
}
