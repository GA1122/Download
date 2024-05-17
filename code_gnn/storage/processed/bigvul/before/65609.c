nfsd4_lock(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	   struct nfsd4_lock *lock)
{
	struct nfs4_openowner *open_sop = NULL;
	struct nfs4_lockowner *lock_sop = NULL;
	struct nfs4_ol_stateid *lock_stp = NULL;
	struct nfs4_ol_stateid *open_stp = NULL;
	struct nfs4_file *fp;
	struct file *filp = NULL;
	struct nfsd4_blocked_lock *nbl = NULL;
	struct file_lock *file_lock = NULL;
	struct file_lock *conflock = NULL;
	__be32 status = 0;
	int lkflg;
	int err;
	bool new = false;
	unsigned char fl_type;
	unsigned int fl_flags = FL_POSIX;
	struct net *net = SVC_NET(rqstp);
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);

	dprintk("NFSD: nfsd4_lock: start=%Ld length=%Ld\n",
		(long long) lock->lk_offset,
		(long long) lock->lk_length);

	if (check_lock_length(lock->lk_offset, lock->lk_length))
		 return nfserr_inval;

	if ((status = fh_verify(rqstp, &cstate->current_fh,
				S_IFREG, NFSD_MAY_LOCK))) {
		dprintk("NFSD: nfsd4_lock: permission denied!\n");
		return status;
	}

	if (lock->lk_is_new) {
		if (nfsd4_has_session(cstate))
			 
			memcpy(&lock->lk_new_clientid,
				&cstate->session->se_client->cl_clientid,
				sizeof(clientid_t));

		status = nfserr_stale_clientid;
		if (STALE_CLIENTID(&lock->lk_new_clientid, nn))
			goto out;

		 
		status = nfs4_preprocess_confirmed_seqid_op(cstate,
				        lock->lk_new_open_seqid,
		                        &lock->lk_new_open_stateid,
					&open_stp, nn);
		if (status)
			goto out;
		mutex_unlock(&open_stp->st_mutex);
		open_sop = openowner(open_stp->st_stateowner);
		status = nfserr_bad_stateid;
		if (!same_clid(&open_sop->oo_owner.so_client->cl_clientid,
						&lock->lk_new_clientid))
			goto out;
		status = lookup_or_create_lock_state(cstate, open_stp, lock,
							&lock_stp, &new);
	} else {
		status = nfs4_preprocess_seqid_op(cstate,
				       lock->lk_old_lock_seqid,
				       &lock->lk_old_lock_stateid,
				       NFS4_LOCK_STID, &lock_stp, nn);
	}
	if (status)
		goto out;
	lock_sop = lockowner(lock_stp->st_stateowner);

	lkflg = setlkflg(lock->lk_type);
	status = nfs4_check_openmode(lock_stp, lkflg);
	if (status)
		goto out;

	status = nfserr_grace;
	if (locks_in_grace(net) && !lock->lk_reclaim)
		goto out;
	status = nfserr_no_grace;
	if (!locks_in_grace(net) && lock->lk_reclaim)
		goto out;

	fp = lock_stp->st_stid.sc_file;
	switch (lock->lk_type) {
		case NFS4_READW_LT:
			if (nfsd4_has_session(cstate))
				fl_flags |= FL_SLEEP;
			 
		case NFS4_READ_LT:
			spin_lock(&fp->fi_lock);
			filp = find_readable_file_locked(fp);
			if (filp)
				get_lock_access(lock_stp, NFS4_SHARE_ACCESS_READ);
			spin_unlock(&fp->fi_lock);
			fl_type = F_RDLCK;
			break;
		case NFS4_WRITEW_LT:
			if (nfsd4_has_session(cstate))
				fl_flags |= FL_SLEEP;
			 
		case NFS4_WRITE_LT:
			spin_lock(&fp->fi_lock);
			filp = find_writeable_file_locked(fp);
			if (filp)
				get_lock_access(lock_stp, NFS4_SHARE_ACCESS_WRITE);
			spin_unlock(&fp->fi_lock);
			fl_type = F_WRLCK;
			break;
		default:
			status = nfserr_inval;
		goto out;
	}

	if (!filp) {
		status = nfserr_openmode;
		goto out;
	}

	nbl = find_or_allocate_block(lock_sop, &fp->fi_fhandle, nn);
	if (!nbl) {
		dprintk("NFSD: %s: unable to allocate block!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}

	file_lock = &nbl->nbl_lock;
	file_lock->fl_type = fl_type;
	file_lock->fl_owner = (fl_owner_t)lockowner(nfs4_get_stateowner(&lock_sop->lo_owner));
	file_lock->fl_pid = current->tgid;
	file_lock->fl_file = filp;
	file_lock->fl_flags = fl_flags;
	file_lock->fl_lmops = &nfsd_posix_mng_ops;
	file_lock->fl_start = lock->lk_offset;
	file_lock->fl_end = last_byte_offset(lock->lk_offset, lock->lk_length);
	nfs4_transform_lock_offset(file_lock);

	conflock = locks_alloc_lock();
	if (!conflock) {
		dprintk("NFSD: %s: unable to allocate lock!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}

	if (fl_flags & FL_SLEEP) {
		nbl->nbl_time = jiffies;
		spin_lock(&nn->blocked_locks_lock);
		list_add_tail(&nbl->nbl_list, &lock_sop->lo_blocked);
		list_add_tail(&nbl->nbl_lru, &nn->blocked_locks_lru);
		spin_unlock(&nn->blocked_locks_lock);
	}

	err = vfs_lock_file(filp, F_SETLK, file_lock, conflock);
	switch (err) {
	case 0:  
		nfs4_inc_and_copy_stateid(&lock->lk_resp_stateid, &lock_stp->st_stid);
		status = 0;
		break;
	case FILE_LOCK_DEFERRED:
		nbl = NULL;
		 
	case -EAGAIN:		 
		status = nfserr_denied;
		dprintk("NFSD: nfsd4_lock: conflicting lock found!\n");
		nfs4_set_lock_denied(conflock, &lock->lk_denied);
		break;
	case -EDEADLK:
		status = nfserr_deadlock;
		break;
	default:
		dprintk("NFSD: nfsd4_lock: vfs_lock_file() failed! status %d\n",err);
		status = nfserrno(err);
		break;
	}
out:
	if (nbl) {
		 
		if (fl_flags & FL_SLEEP) {
			spin_lock(&nn->blocked_locks_lock);
			list_del_init(&nbl->nbl_list);
			list_del_init(&nbl->nbl_lru);
			spin_unlock(&nn->blocked_locks_lock);
		}
		free_blocked_lock(nbl);
	}
	if (filp)
		fput(filp);
	if (lock_stp) {
		 
		if (cstate->replay_owner &&
		    cstate->replay_owner != &lock_sop->lo_owner &&
		    seqid_mutating_err(ntohl(status)))
			lock_sop->lo_owner.so_seqid++;

		mutex_unlock(&lock_stp->st_mutex);

		 
		if (status && new)
			release_lock_stateid(lock_stp);

		nfs4_put_stid(&lock_stp->st_stid);
	}
	if (open_stp)
		nfs4_put_stid(&open_stp->st_stid);
	nfsd4_bump_seqid(cstate, status);
	if (conflock)
		locks_free_lock(conflock);
	return status;
}