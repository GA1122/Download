int hostap_set_encryption(local_info_t *local)
{
	u16 val, old_val;
	int i, keylen, len, idx;
	char keybuf[WEP_KEY_LEN + 1];
	enum { NONE, WEP, OTHER } encrypt_type;

	idx = local->crypt_info.tx_keyidx;
	if (local->crypt_info.crypt[idx] == NULL ||
	    local->crypt_info.crypt[idx]->ops == NULL)
		encrypt_type = NONE;
	else if (strcmp(local->crypt_info.crypt[idx]->ops->name, "WEP") == 0)
		encrypt_type = WEP;
	else
		encrypt_type = OTHER;

	if (local->func->get_rid(local->dev, HFA384X_RID_CNFWEPFLAGS, &val, 2,
				 1) < 0) {
		printk(KERN_DEBUG "Could not read current WEP flags.\n");
		goto fail;
	}
	le16_to_cpus(&val);
	old_val = val;

	if (encrypt_type != NONE || local->privacy_invoked)
		val |= HFA384X_WEPFLAGS_PRIVACYINVOKED;
	else
		val &= ~HFA384X_WEPFLAGS_PRIVACYINVOKED;

	if (local->open_wep || encrypt_type == NONE ||
	    ((local->ieee_802_1x || local->wpa) && local->host_decrypt))
		val &= ~HFA384X_WEPFLAGS_EXCLUDEUNENCRYPTED;
	else
		val |= HFA384X_WEPFLAGS_EXCLUDEUNENCRYPTED;

	if ((encrypt_type != NONE || local->privacy_invoked) &&
	    (encrypt_type == OTHER || local->host_encrypt))
		val |= HFA384X_WEPFLAGS_HOSTENCRYPT;
	else
		val &= ~HFA384X_WEPFLAGS_HOSTENCRYPT;
	if ((encrypt_type != NONE || local->privacy_invoked) &&
	    (encrypt_type == OTHER || local->host_decrypt))
		val |= HFA384X_WEPFLAGS_HOSTDECRYPT;
	else
		val &= ~HFA384X_WEPFLAGS_HOSTDECRYPT;

	if (val != old_val &&
	    hostap_set_word(local->dev, HFA384X_RID_CNFWEPFLAGS, val)) {
		printk(KERN_DEBUG "Could not write new WEP flags (0x%x)\n",
		       val);
		goto fail;
	}

	if (encrypt_type != WEP)
		return 0;

	 
	keylen = 6;  
	len = local->crypt_info.crypt[idx]->ops->get_key(keybuf, sizeof(keybuf), NULL,
							   local->crypt_info.crypt[idx]->priv);
	if (idx >= 0 && idx < WEP_KEYS && len > 5)
		keylen = WEP_KEY_LEN + 1;  

	for (i = 0; i < WEP_KEYS; i++) {
		memset(keybuf, 0, sizeof(keybuf));
		if (local->crypt_info.crypt[i]) {
			(void) local->crypt_info.crypt[i]->ops->get_key(
				keybuf, sizeof(keybuf),
				NULL, local->crypt_info.crypt[i]->priv);
		}
		if (local->func->set_rid(local->dev,
					 HFA384X_RID_CNFDEFAULTKEY0 + i,
					 keybuf, keylen)) {
			printk(KERN_DEBUG "Could not set key %d (len=%d)\n",
			       i, keylen);
			goto fail;
		}
	}
	if (hostap_set_word(local->dev, HFA384X_RID_CNFWEPDEFAULTKEYID, idx)) {
		printk(KERN_DEBUG "Could not set default keyid %d\n", idx);
		goto fail;
	}

	return 0;

 fail:
	printk(KERN_DEBUG "%s: encryption setup failed\n", local->dev->name);
	return -1;
}