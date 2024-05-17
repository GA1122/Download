static struct nfs4_state *nfs4_do_open(struct inode *dir,
					struct nfs_open_context *ctx,
					int flags,
					struct iattr *sattr,
					struct nfs4_label *label,
					int *opened)
{
	struct nfs_server *server = NFS_SERVER(dir);
	struct nfs4_exception exception = { };
	struct nfs4_state *res;
	int status;

	do {
		status = _nfs4_do_open(dir, ctx, flags, sattr, label, opened);
		res = ctx->state;
		trace_nfs4_open_file(ctx, flags, status);
		if (status == 0)
			break;
		 
		if (status == -NFS4ERR_BAD_SEQID) {
			pr_warn_ratelimited("NFS: v4 server %s "
					" returned a bad sequence-id error!\n",
					NFS_SERVER(dir)->nfs_client->cl_hostname);
			exception.retry = 1;
			continue;
		}
		 
		if (status == -NFS4ERR_BAD_STATEID) {
			exception.retry = 1;
			continue;
		}
		if (status == -EAGAIN) {
			 
			exception.retry = 1;
			continue;
		}
		if (nfs4_clear_cap_atomic_open_v1(server, status, &exception))
			continue;
		res = ERR_PTR(nfs4_handle_exception(server,
					status, &exception));
	} while (exception.retry);
	return res;
}
