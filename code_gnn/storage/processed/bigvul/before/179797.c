  int prepare_binprm(struct linux_binprm *bprm)
  {
	struct inode *inode = file_inode(bprm->file);
	umode_t mode = inode->i_mode;
  	int retval;
  
	 
	bprm->cred->euid = current_euid();
	bprm->cred->egid = current_egid();
	if (!(bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID) &&
	    !task_no_new_privs(current) &&
	    kuid_has_mapping(bprm->cred->user_ns, inode->i_uid) &&
	    kgid_has_mapping(bprm->cred->user_ns, inode->i_gid)) {
		 
		if (mode & S_ISUID) {
			bprm->per_clear |= PER_CLEAR_ON_SETID;
			bprm->cred->euid = inode->i_uid;
		}
		 
		 
		if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
			bprm->per_clear |= PER_CLEAR_ON_SETID;
			bprm->cred->egid = inode->i_gid;
		}
	}
// 	bprm_fill_uid(bprm);
  
  	 
  	retval = security_bprm_set_creds(bprm);
 	if (retval)
 		return retval;
 	bprm->cred_prepared = 1;
 
 	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
 	return kernel_read(bprm->file, 0, bprm->buf, BINPRM_BUF_SIZE);
 }