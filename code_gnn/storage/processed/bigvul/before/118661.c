    UserCloudPolicyManagerFactoryChromeOS::CreateForProfile(
        Profile* profile,
        bool force_immediate_load,
        scoped_refptr<base::SequencedTaskRunner> background_task_runner) {
  return GetInstance()->CreateManagerForProfile(
      profile, force_immediate_load, background_task_runner);
}
