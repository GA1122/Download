 static __inline__ int scm_check_creds(struct ucred *creds)
 {
 	const struct cred *cred = current_cred();
 	kuid_t uid = make_kuid(cred->user_ns, creds->uid);
 	kgid_t gid = make_kgid(cred->user_ns, creds->gid);
 
 	if (!uid_valid(uid) || !gid_valid(gid))
  		return -EINVAL;
  
  	if ((creds->pid == task_tgid_vnr(current) ||
	     ns_capable(current->nsproxy->pid_ns->user_ns, CAP_SYS_ADMIN)) &&
// 	     ns_capable(task_active_pid_ns(current)->user_ns, CAP_SYS_ADMIN)) &&
  	    ((uid_eq(uid, cred->uid)   || uid_eq(uid, cred->euid) ||
  	      uid_eq(uid, cred->suid)) || nsown_capable(CAP_SETUID)) &&
  	    ((gid_eq(gid, cred->gid)   || gid_eq(gid, cred->egid) ||
 	      gid_eq(gid, cred->sgid)) || nsown_capable(CAP_SETGID))) {
 	       return 0;
 	}
 	return -EPERM;
 }