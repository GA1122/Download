nfsd4_decode_lock(struct nfsd4_compoundargs *argp, struct nfsd4_lock *lock)
{
	DECODE_HEAD;

	 
	READ_BUF(28);
	lock->lk_type = be32_to_cpup(p++);
	if ((lock->lk_type < NFS4_READ_LT) || (lock->lk_type > NFS4_WRITEW_LT))
		goto xdr_error;
	lock->lk_reclaim = be32_to_cpup(p++);
	p = xdr_decode_hyper(p, &lock->lk_offset);
	p = xdr_decode_hyper(p, &lock->lk_length);
	lock->lk_is_new = be32_to_cpup(p++);

	if (lock->lk_is_new) {
		READ_BUF(4);
		lock->lk_new_open_seqid = be32_to_cpup(p++);
		status = nfsd4_decode_stateid(argp, &lock->lk_new_open_stateid);
		if (status)
			return status;
		READ_BUF(8 + sizeof(clientid_t));
		lock->lk_new_lock_seqid = be32_to_cpup(p++);
		COPYMEM(&lock->lk_new_clientid, sizeof(clientid_t));
		lock->lk_new_owner.len = be32_to_cpup(p++);
		READ_BUF(lock->lk_new_owner.len);
		READMEM(lock->lk_new_owner.data, lock->lk_new_owner.len);
	} else {
		status = nfsd4_decode_stateid(argp, &lock->lk_old_lock_stateid);
		if (status)
			return status;
		READ_BUF(4);
		lock->lk_old_lock_seqid = be32_to_cpup(p++);
	}

	DECODE_TAIL;
}