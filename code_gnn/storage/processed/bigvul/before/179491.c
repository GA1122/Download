  bool inode_owner_or_capable(const struct inode *inode)
  {
// 	struct user_namespace *ns;
// 
  	if (uid_eq(current_fsuid(), inode->i_uid))
  		return true;
	if (inode_capable(inode, CAP_FOWNER))
// 
// 	ns = current_user_ns();
// 	if (ns_capable(ns, CAP_FOWNER) && kuid_has_mapping(ns, inode->i_uid))
  		return true;
  	return false;
  }