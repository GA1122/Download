static int _nfs4_proc_setlk(struct nfs4_state *state, int cmd, struct file_lock *request)
{
	struct nfs_inode *nfsi = NFS_I(state->inode);
	unsigned char fl_flags = request->fl_flags;
	int status = -ENOLCK;

	if ((fl_flags & FL_POSIX) &&
			!test_bit(NFS_STATE_POSIX_LOCKS, &state->flags))
		goto out;
	 
	status = nfs4_set_lock_state(state, request);
	if (status != 0)
		goto out;
	request->fl_flags |= FL_ACCESS;
	status = do_vfs_lock(state->inode, request);
	if (status < 0)
		goto out;
	down_read(&nfsi->rwsem);
	if (test_bit(NFS_DELEGATED_STATE, &state->flags)) {
		 
		 
		request->fl_flags = fl_flags & ~FL_SLEEP;
		status = do_vfs_lock(state->inode, request);
		up_read(&nfsi->rwsem);
		goto out;
	}
	up_read(&nfsi->rwsem);
	status = _nfs4_do_setlk(state, cmd, request, NFS_LOCK_NEW);
out:
	request->fl_flags = fl_flags;
	return status;
}