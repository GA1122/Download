DictionaryValue* CreateDictionaryWithPolicies(
    policy::CloudPolicySubsystem* policy_subsystem,
    policy::CloudPolicyCacheBase::PolicyLevel policy_level) {
  DictionaryValue* dict = new DictionaryValue;
  policy::CloudPolicyCacheBase* policy_cache =
      policy_subsystem->GetCloudPolicyCacheBase();
  if (policy_cache) {
    const policy::PolicyMap* policy_map = policy_cache->policy(policy_level);
    if (policy_map) {
      policy::PolicyMap::const_iterator i;
      for (i = policy_map->begin(); i != policy_map->end(); i++)
        dict->Set(policy::GetPolicyName(i->first),
                  i->second->DeepCopy());
    }
  }
  return dict;
}
