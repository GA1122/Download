SYSCALL_DEFINE3(getresgid, gid_t __user *, rgid, gid_t __user *, egid, gid_t __user *, sgid)
{
 const struct cred *cred = current_cred();
 int retval;

 if (!(retval   = put_user(cred->gid,  rgid)) &&
 !(retval   = put_user(cred->egid, egid)))
		retval = put_user(cred->sgid, sgid);

 return retval;
}
