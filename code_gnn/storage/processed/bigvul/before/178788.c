 int cap_bprm_set_creds(struct linux_binprm *bprm)
 {
 	const struct cred *old = current_cred();
 	struct cred *new = bprm->cred;
 	bool effective, has_cap = false;
 	int ret;
 
 	effective = false;
 	ret = get_file_caps(bprm, &effective, &has_cap);
 	if (ret < 0)
 		return ret;
 
 	if (!issecure(SECURE_NOROOT)) {
 		 
 		if (has_cap && new->uid != 0 && new->euid == 0) {
 			warn_setuid_and_fcaps_mixed(bprm->filename);
 			goto skip;
 		}
 		 
 		if (new->euid == 0 || new->uid == 0) {
 			 
 			new->cap_permitted = cap_combine(old->cap_bset,
 							 old->cap_inheritable);
 		}
 		if (new->euid == 0)
 			effective = true;
  	}
  skip:
  
// 	 
// 	if (!cap_issubset(new->cap_permitted, old->cap_permitted))
// 		bprm->per_clear |= PER_CLEAR_ON_SETID;
// 
// 
  	 
 	if ((new->euid != old->uid ||
 	     new->egid != old->gid ||
 	     !cap_issubset(new->cap_permitted, old->cap_permitted)) &&
 	    bprm->unsafe & ~LSM_UNSAFE_PTRACE_CAP) {
 		 
 		if (!capable(CAP_SETUID)) {
 			new->euid = new->uid;
 			new->egid = new->gid;
 		}
 		new->cap_permitted = cap_intersect(new->cap_permitted,
 						   old->cap_permitted);
 	}
 
 	new->suid = new->fsuid = new->euid;
 	new->sgid = new->fsgid = new->egid;
 
 	if (effective)
 		new->cap_effective = new->cap_permitted;
 	else
 		cap_clear(new->cap_effective);
 	bprm->cap_effective = effective;
 
 	 
 	if (!cap_isclear(new->cap_effective)) {
 		if (!cap_issubset(CAP_FULL_SET, new->cap_effective) ||
 		    new->euid != 0 || new->uid != 0 ||
 		    issecure(SECURE_NOROOT)) {
 			ret = audit_log_bprm_fcaps(bprm, new, old);
 			if (ret < 0)
 				return ret;
 		}
 	}
 
 	new->securebits &= ~issecure_mask(SECURE_KEEP_CAPS);
 	return 0;
 }