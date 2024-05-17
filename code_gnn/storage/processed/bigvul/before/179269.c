 int create_user_ns(struct cred *new)
 {
 	struct user_namespace *ns, *parent_ns = new->user_ns;
 	kuid_t owner = new->euid;
  	kgid_t group = new->egid;
  	int ret;
  
// 	 
// 	if (current_chrooted())
// 		return -EPERM;
// 
  	 
 	if (!kuid_has_mapping(parent_ns, owner) ||
 	    !kgid_has_mapping(parent_ns, group))
 		return -EPERM;
 
 	ns = kmem_cache_zalloc(user_ns_cachep, GFP_KERNEL);
 	if (!ns)
 		return -ENOMEM;
 
 	ret = proc_alloc_inum(&ns->proc_inum);
 	if (ret) {
 		kmem_cache_free(user_ns_cachep, ns);
 		return ret;
 	}
 
 	atomic_set(&ns->count, 1);
 	 
 	ns->parent = parent_ns;
 	ns->owner = owner;
 	ns->group = group;
 
 	set_cred_user_ns(new, ns);
 
 	return 0;
 }