  void UpdateProviderPolicy(const PolicyMap& policy) {
    PolicyMap policy_with_defaults;
    policy_with_defaults.CopyFrom(policy);
#if defined(OS_CHROMEOS)
    SetEnterpriseUsersDefaults(&policy_with_defaults);
#endif
    provider_.UpdateChromePolicy(policy_with_defaults);
    DCHECK(base::MessageLoopCurrent::Get());
    base::RunLoop loop;
    loop.RunUntilIdle();
  }
