bool AddPolicyForGPU(CommandLine* cmd_line, sandbox::TargetPolicy* policy) {
#if !defined(NACL_WIN64)   
  if (base::win::GetVersion() > base::win::VERSION_XP) {
    if (cmd_line->GetSwitchValueASCII(switches::kUseGL) ==
        gfx::kGLImplementationDesktopName) {
      policy->SetTokenLevel(sandbox::USER_RESTRICTED_SAME_ACCESS,
                            sandbox::USER_LIMITED);
      policy->SetJobLevel(sandbox::JOB_UNPROTECTED, 0);
      policy->SetDelayedIntegrityLevel(sandbox::INTEGRITY_LEVEL_LOW);
    } else {
      if (cmd_line->GetSwitchValueASCII(switches::kUseGL) ==
          gfx::kGLImplementationSwiftShaderName ||
          cmd_line->HasSwitch(switches::kReduceGpuSandbox) ||
          cmd_line->HasSwitch(switches::kDisableImageTransportSurface)) {
        policy->SetTokenLevel(sandbox::USER_RESTRICTED_SAME_ACCESS,
                              sandbox::USER_LIMITED);
      } else {
        policy->SetTokenLevel(sandbox::USER_RESTRICTED_SAME_ACCESS,
                              sandbox::USER_RESTRICTED);
        policy->SetDelayedIntegrityLevel(sandbox::INTEGRITY_LEVEL_UNTRUSTED);
      }

      policy->SetJobLevel(sandbox::JOB_LIMITED_USER,
                          JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS |
                          JOB_OBJECT_UILIMIT_DESKTOP |
                          JOB_OBJECT_UILIMIT_EXITWINDOWS |
                          JOB_OBJECT_UILIMIT_DISPLAYSETTINGS);

      policy->SetIntegrityLevel(sandbox::INTEGRITY_LEVEL_LOW);
    }
  } else {
    policy->SetJobLevel(sandbox::JOB_UNPROTECTED, 0);
    policy->SetTokenLevel(sandbox::USER_UNPROTECTED,
                          sandbox::USER_LIMITED);
  }

  sandbox::ResultCode result = policy->AddRule(
      sandbox::TargetPolicy::SUBSYS_NAMED_PIPES,
      sandbox::TargetPolicy::NAMEDPIPES_ALLOW_ANY,
      L"\\\\.\\pipe\\chrome.gpu.*");
  if (result != sandbox::SBOX_ALL_OK)
    return false;

  result = policy->AddRule(sandbox::TargetPolicy::SUBSYS_HANDLES,
                           sandbox::TargetPolicy::HANDLES_DUP_ANY,
                           L"Section");
  if (result != sandbox::SBOX_ALL_OK)
    return false;

#ifdef USE_AURA
  result = policy->AddRule(sandbox::TargetPolicy::SUBSYS_HANDLES,
                           sandbox::TargetPolicy::HANDLES_DUP_BROKER,
                           L"Section");
  if (result != sandbox::SBOX_ALL_OK)
    return false;
#endif

  AddGenericDllEvictionPolicy(policy);
#endif
  return true;
}
