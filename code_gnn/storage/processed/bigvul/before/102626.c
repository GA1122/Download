 const PolicyMap* CloudPolicyCacheBase::policy(PolicyLevel level) {
   switch (level) {
     case POLICY_LEVEL_MANDATORY:
      return &mandatory_policy_;
    case POLICY_LEVEL_RECOMMENDED:
      return &recommended_policy_;
  }
  NOTREACHED();
  return NULL;
}
