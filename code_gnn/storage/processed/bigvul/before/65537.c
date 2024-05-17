static __be32 nfs4_seqid_op_checks(struct nfsd4_compound_state *cstate, stateid_t *stateid, u32 seqid, struct nfs4_ol_stateid *stp)
{
	struct svc_fh *current_fh = &cstate->current_fh;
	struct nfs4_stateowner *sop = stp->st_stateowner;
	__be32 status;

	status = nfsd4_check_seqid(cstate, sop, seqid);
	if (status)
		return status;
	if (stp->st_stid.sc_type == NFS4_CLOSED_STID
		|| stp->st_stid.sc_type == NFS4_REVOKED_DELEG_STID)
		 
		return nfserr_bad_stateid;
	mutex_lock(&stp->st_mutex);
	status = check_stateid_generation(stateid, &stp->st_stid.sc_stateid, nfsd4_has_session(cstate));
	if (status == nfs_ok)
		status = nfs4_check_fh(current_fh, &stp->st_stid);
	if (status != nfs_ok)
		mutex_unlock(&stp->st_mutex);
	return status;
}