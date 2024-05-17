int hostap_set_auth_algs(local_info_t *local)
{
	int val = local->auth_algs;
	 
	if (local->sta_fw_ver < PRISM2_FW_VER(0,7,0) &&
	    val != PRISM2_AUTH_OPEN && val != PRISM2_AUTH_SHARED_KEY)
		val = PRISM2_AUTH_OPEN;

	if (hostap_set_word(local->dev, HFA384X_RID_CNFAUTHENTICATION, val)) {
		printk(KERN_INFO "%s: cnfAuthentication setting to 0x%x "
		       "failed\n", local->dev->name, local->auth_algs);
		return -EINVAL;
	}

	return 0;
}
