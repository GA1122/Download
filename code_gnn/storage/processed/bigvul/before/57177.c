static struct nfs4_opendata *nfs4_opendata_alloc(struct dentry *dentry,
		struct nfs4_state_owner *sp, fmode_t fmode, int flags,
		const struct iattr *attrs,
		struct nfs4_label *label,
		enum open_claim_type4 claim,
		gfp_t gfp_mask)
{
	struct dentry *parent = dget_parent(dentry);
	struct inode *dir = d_inode(parent);
	struct nfs_server *server = NFS_SERVER(dir);
	struct nfs_seqid *(*alloc_seqid)(struct nfs_seqid_counter *, gfp_t);
	struct nfs4_opendata *p;

	p = kzalloc(sizeof(*p), gfp_mask);
	if (p == NULL)
		goto err;

	p->f_label = nfs4_label_alloc(server, gfp_mask);
	if (IS_ERR(p->f_label))
		goto err_free_p;

	p->a_label = nfs4_label_alloc(server, gfp_mask);
	if (IS_ERR(p->a_label))
		goto err_free_f;

	alloc_seqid = server->nfs_client->cl_mvops->alloc_seqid;
	p->o_arg.seqid = alloc_seqid(&sp->so_seqid, gfp_mask);
	if (IS_ERR(p->o_arg.seqid))
		goto err_free_label;
	nfs_sb_active(dentry->d_sb);
	p->dentry = dget(dentry);
	p->dir = parent;
	p->owner = sp;
	atomic_inc(&sp->so_count);
	p->o_arg.open_flags = flags;
	p->o_arg.fmode = fmode & (FMODE_READ|FMODE_WRITE);
	p->o_arg.share_access = nfs4_map_atomic_open_share(server,
			fmode, flags);
	 
	if (!(flags & O_EXCL)) {
		 
		p->o_arg.access = NFS4_ACCESS_READ | NFS4_ACCESS_MODIFY |
				  NFS4_ACCESS_EXTEND | NFS4_ACCESS_EXECUTE;
	}
	p->o_arg.clientid = server->nfs_client->cl_clientid;
	p->o_arg.id.create_time = ktime_to_ns(sp->so_seqid.create_time);
	p->o_arg.id.uniquifier = sp->so_seqid.owner_id;
	p->o_arg.name = &dentry->d_name;
	p->o_arg.server = server;
	p->o_arg.bitmask = nfs4_bitmask(server, label);
	p->o_arg.open_bitmap = &nfs4_fattr_bitmap[0];
	p->o_arg.label = nfs4_label_copy(p->a_label, label);
	p->o_arg.claim = nfs4_map_atomic_open_claim(server, claim);
	switch (p->o_arg.claim) {
	case NFS4_OPEN_CLAIM_NULL:
	case NFS4_OPEN_CLAIM_DELEGATE_CUR:
	case NFS4_OPEN_CLAIM_DELEGATE_PREV:
		p->o_arg.fh = NFS_FH(dir);
		break;
	case NFS4_OPEN_CLAIM_PREVIOUS:
	case NFS4_OPEN_CLAIM_FH:
	case NFS4_OPEN_CLAIM_DELEG_CUR_FH:
	case NFS4_OPEN_CLAIM_DELEG_PREV_FH:
		p->o_arg.fh = NFS_FH(d_inode(dentry));
	}
	if (attrs != NULL && attrs->ia_valid != 0) {
		__u32 verf[2];

		p->o_arg.u.attrs = &p->attrs;
		memcpy(&p->attrs, attrs, sizeof(p->attrs));

		verf[0] = jiffies;
		verf[1] = current->pid;
		memcpy(p->o_arg.u.verifier.data, verf,
				sizeof(p->o_arg.u.verifier.data));
	}
	p->c_arg.fh = &p->o_res.fh;
	p->c_arg.stateid = &p->o_res.stateid;
	p->c_arg.seqid = p->o_arg.seqid;
	nfs4_init_opendata_res(p);
	kref_init(&p->kref);
	return p;

err_free_label:
	nfs4_label_free(p->a_label);
err_free_f:
	nfs4_label_free(p->f_label);
err_free_p:
	kfree(p);
err:
	dput(parent);
	return NULL;
}