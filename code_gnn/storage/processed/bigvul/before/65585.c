nfsd4_find_existing_open(struct nfs4_file *fp, struct nfsd4_open *open)
{
	struct nfs4_ol_stateid *local, *ret = NULL;
	struct nfs4_openowner *oo = open->op_openowner;

	lockdep_assert_held(&fp->fi_lock);

	list_for_each_entry(local, &fp->fi_stateids, st_perfile) {
		 
		if (local->st_stateowner->so_is_open_owner == 0)
			continue;
		if (local->st_stateowner == &oo->oo_owner) {
			ret = local;
			atomic_inc(&ret->st_stid.sc_count);
			break;
		}
	}
	return ret;
}