bool FileSystemPolicy::SetInitialRules(LowLevelPolicy* policy) {
  PolicyRule format(ASK_BROKER);
  PolicyRule short_name(ASK_BROKER);

  bool rv = format.AddNumberMatch(IF_NOT, FileName::BROKER, TRUE, AND);
  rv &= format.AddStringMatch(IF_NOT, FileName::NAME, L"\\/?/?\\*",
                              CASE_SENSITIVE);

  rv &= short_name.AddNumberMatch(IF_NOT, FileName::BROKER, TRUE, AND);
  rv &= short_name.AddStringMatch(IF, FileName::NAME, L"*~*", CASE_SENSITIVE);

  if (!rv || !policy->AddRule(IPC_NTCREATEFILE_TAG, &format))
    return false;

  if (!policy->AddRule(IPC_NTCREATEFILE_TAG, &short_name))
    return false;

  if (!policy->AddRule(IPC_NTOPENFILE_TAG, &format))
    return false;

  if (!policy->AddRule(IPC_NTOPENFILE_TAG, &short_name))
    return false;

  if (!policy->AddRule(IPC_NTQUERYATTRIBUTESFILE_TAG, &format))
    return false;

  if (!policy->AddRule(IPC_NTQUERYATTRIBUTESFILE_TAG, &short_name))
    return false;

  if (!policy->AddRule(IPC_NTQUERYFULLATTRIBUTESFILE_TAG, &format))
    return false;

  if (!policy->AddRule(IPC_NTQUERYFULLATTRIBUTESFILE_TAG, &short_name))
    return false;

  if (!policy->AddRule(IPC_NTSETINFO_RENAME_TAG, &format))
    return false;

  if (!policy->AddRule(IPC_NTSETINFO_RENAME_TAG, &short_name))
    return false;

  return true;
}
