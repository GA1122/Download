static int audit_set_loginuid_perm(kuid_t loginuid)
{
	 
	if (!audit_loginuid_set(current))
		return 0;
	 
	if (is_audit_feature_set(AUDIT_FEATURE_LOGINUID_IMMUTABLE))
		return -EPERM;
	 
	if (!capable(CAP_AUDIT_CONTROL))
		return -EPERM;
	 
	if (is_audit_feature_set(AUDIT_FEATURE_ONLY_UNSET_LOGINUID) && uid_valid(loginuid))
		return -EPERM;
	return 0;
}
