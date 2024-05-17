 static struct mnt_namespace *alloc_mnt_ns(struct user_namespace *user_ns)
 {
 	struct mnt_namespace *new_ns;
 	struct ucounts *ucounts;
 	int ret;
 
 	ucounts = inc_mnt_namespaces(user_ns);
 	if (!ucounts)
 		return ERR_PTR(-ENOSPC);
 
 	new_ns = kmalloc(sizeof(struct mnt_namespace), GFP_KERNEL);
 	if (!new_ns) {
 		dec_mnt_namespaces(ucounts);
 		return ERR_PTR(-ENOMEM);
 	}
 	ret = ns_alloc_inum(&new_ns->ns);
 	if (ret) {
 		kfree(new_ns);
 		dec_mnt_namespaces(ucounts);
 		return ERR_PTR(ret);
 	}
 	new_ns->ns.ops = &mntns_operations;
 	new_ns->seq = atomic64_add_return(1, &mnt_ns_seq);
 	atomic_set(&new_ns->count, 1);
 	new_ns->root = NULL;
 	INIT_LIST_HEAD(&new_ns->list);
 	init_waitqueue_head(&new_ns->poll);
  	new_ns->event = 0;
  	new_ns->user_ns = get_user_ns(user_ns);
  	new_ns->ucounts = ucounts;
// 	new_ns->mounts = 0;
// 	new_ns->pending_mounts = 0;
  	return new_ns;
  }