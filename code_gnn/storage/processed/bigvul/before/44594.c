struct lxc_conf *lxc_conf_init(void)
{
	struct lxc_conf *new;
	int i;

	new = 	malloc(sizeof(*new));
	if (!new) {
		ERROR("lxc_conf_init : %m");
		return NULL;
	}
	memset(new, 0, sizeof(*new));

	new->loglevel = LXC_LOG_PRIORITY_NOTSET;
	new->personality = -1;
	new->autodev = 1;
	new->console.log_path = NULL;
	new->console.log_fd = -1;
	new->console.path = NULL;
	new->console.peer = -1;
	new->console.peerpty.busy = -1;
	new->console.peerpty.master = -1;
	new->console.peerpty.slave = -1;
	new->console.master = -1;
	new->console.slave = -1;
	new->console.name[0] = '\0';
	new->maincmd_fd = -1;
	new->nbd_idx = -1;
	new->rootfs.mount = strdup(default_rootfs_mount);
	if (!new->rootfs.mount) {
		ERROR("lxc_conf_init : %m");
		free(new);
		return NULL;
	}
	new->kmsg = 0;
	new->logfd = -1;
	lxc_list_init(&new->cgroup);
	lxc_list_init(&new->network);
	lxc_list_init(&new->mount_list);
	lxc_list_init(&new->caps);
	lxc_list_init(&new->keepcaps);
	lxc_list_init(&new->id_map);
	lxc_list_init(&new->includes);
	lxc_list_init(&new->aliens);
	lxc_list_init(&new->environment);
	for (i=0; i<NUM_LXC_HOOKS; i++)
		lxc_list_init(&new->hooks[i]);
	lxc_list_init(&new->groups);
	new->lsm_aa_profile = NULL;
	new->lsm_se_context = NULL;
	new->tmp_umount_proc = 0;

	for (i = 0; i < LXC_NS_MAX; i++)
		new->inherit_ns_fd[i] = -1;

	 
	new->init_uid = 0;
	new->init_gid = 0;

	return new;
}