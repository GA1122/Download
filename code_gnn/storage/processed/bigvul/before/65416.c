check_for_locks(struct nfs4_file *fp, struct nfs4_lockowner *lowner)
{
	struct file_lock *fl;
	int status = false;
	struct file *filp = find_any_file(fp);
	struct inode *inode;
	struct file_lock_context *flctx;

	if (!filp) {
		 
		WARN_ON_ONCE(1);
		return status;
	}

	inode = file_inode(filp);
	flctx = inode->i_flctx;

	if (flctx && !list_empty_careful(&flctx->flc_posix)) {
		spin_lock(&flctx->flc_lock);
		list_for_each_entry(fl, &flctx->flc_posix, fl_list) {
			if (fl->fl_owner == (fl_owner_t)lowner) {
				status = true;
				break;
			}
		}
		spin_unlock(&flctx->flc_lock);
	}
	fput(filp);
	return status;
}