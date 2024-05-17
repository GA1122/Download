static __be32 check_stateid_generation(stateid_t *in, stateid_t *ref, bool has_session)
{
	 
	if (has_session && in->si_generation == 0)
		return nfs_ok;

	if (in->si_generation == ref->si_generation)
		return nfs_ok;

	 
	if (nfsd4_stateid_generation_after(in, ref))
		return nfserr_bad_stateid;
	 
	return nfserr_old_stateid;
}
