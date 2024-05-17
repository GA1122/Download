sid_to_id(struct cifs_sb_info *cifs_sb, struct cifs_sid *psid,
		struct cifs_fattr *fattr, uint sidtype)
{
	int rc;
	struct key *sidkey;
	char *sidstr;
	const struct cred *saved_cred;
	kuid_t fuid = cifs_sb->mnt_uid;
	kgid_t fgid = cifs_sb->mnt_gid;

	 
	if (unlikely(psid->num_subauth > SID_MAX_SUB_AUTHORITIES)) {
		cifs_dbg(FYI, "%s: %u subauthorities is too many!\n",
			 __func__, psid->num_subauth);
		return -EIO;
	}

	sidstr = sid_to_key_str(psid, sidtype);
	if (!sidstr)
		return -ENOMEM;

	saved_cred = override_creds(root_cred);
	sidkey = request_key(&cifs_idmap_key_type, sidstr, "");
	if (IS_ERR(sidkey)) {
		rc = -EINVAL;
		cifs_dbg(FYI, "%s: Can't map SID %s to a %cid\n",
			 __func__, sidstr, sidtype == SIDOWNER ? 'u' : 'g');
		goto out_revert_creds;
	}

	 
	BUILD_BUG_ON(sizeof(uid_t) != sizeof(gid_t));
	if (sidkey->datalen != sizeof(uid_t)) {
		rc = -EIO;
		cifs_dbg(FYI, "%s: Downcall contained malformed key (datalen=%hu)\n",
			 __func__, sidkey->datalen);
		key_invalidate(sidkey);
		goto out_key_put;
	}

	if (sidtype == SIDOWNER) {
		kuid_t uid;
		uid_t id;
		memcpy(&id, &sidkey->payload.value, sizeof(uid_t));
		uid = make_kuid(&init_user_ns, id);
		if (uid_valid(uid))
			fuid = uid;
	} else {
		kgid_t gid;
		gid_t id;
		memcpy(&id, &sidkey->payload.value, sizeof(gid_t));
		gid = make_kgid(&init_user_ns, id);
		if (gid_valid(gid))
			fgid = gid;
	}

out_key_put:
	key_put(sidkey);
out_revert_creds:
	revert_creds(saved_cred);
	kfree(sidstr);

	 
	if (sidtype == SIDOWNER)
		fattr->cf_uid = fuid;
	else
		fattr->cf_gid = fgid;
	return 0;
}
