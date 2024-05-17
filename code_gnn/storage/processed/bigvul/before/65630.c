nfsd4_sequence(struct svc_rqst *rqstp,
	       struct nfsd4_compound_state *cstate,
	       struct nfsd4_sequence *seq)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
	struct xdr_stream *xdr = &resp->xdr;
	struct nfsd4_session *session;
	struct nfs4_client *clp;
	struct nfsd4_slot *slot;
	struct nfsd4_conn *conn;
	__be32 status;
	int buflen;
	struct net *net = SVC_NET(rqstp);
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);

	if (resp->opcnt != 1)
		return nfserr_sequence_pos;

	 
	conn = alloc_conn(rqstp, NFS4_CDFC4_FORE);
	if (!conn)
		return nfserr_jukebox;

	spin_lock(&nn->client_lock);
	session = find_in_sessionid_hashtbl(&seq->sessionid, net, &status);
	if (!session)
		goto out_no_session;
	clp = session->se_client;

	status = nfserr_too_many_ops;
	if (nfsd4_session_too_many_ops(rqstp, session))
		goto out_put_session;

	status = nfserr_req_too_big;
	if (nfsd4_request_too_big(rqstp, session))
		goto out_put_session;

	status = nfserr_badslot;
	if (seq->slotid >= session->se_fchannel.maxreqs)
		goto out_put_session;

	slot = session->se_slots[seq->slotid];
	dprintk("%s: slotid %d\n", __func__, seq->slotid);

	 
	seq->maxslots = session->se_fchannel.maxreqs;

	status = check_slot_seqid(seq->seqid, slot->sl_seqid,
					slot->sl_flags & NFSD4_SLOT_INUSE);
	if (status == nfserr_replay_cache) {
		status = nfserr_seq_misordered;
		if (!(slot->sl_flags & NFSD4_SLOT_INITIALIZED))
			goto out_put_session;
		cstate->slot = slot;
		cstate->session = session;
		cstate->clp = clp;
		 
		status = nfsd4_replay_cache_entry(resp, seq);
		cstate->status = nfserr_replay_cache;
		goto out;
	}
	if (status)
		goto out_put_session;

	status = nfsd4_sequence_check_conn(conn, session);
	conn = NULL;
	if (status)
		goto out_put_session;

	buflen = (seq->cachethis) ?
			session->se_fchannel.maxresp_cached :
			session->se_fchannel.maxresp_sz;
	status = (seq->cachethis) ? nfserr_rep_too_big_to_cache :
				    nfserr_rep_too_big;
	if (xdr_restrict_buflen(xdr, buflen - rqstp->rq_auth_slack))
		goto out_put_session;
	svc_reserve(rqstp, buflen);

	status = nfs_ok;
	 
	slot->sl_seqid = seq->seqid;
	slot->sl_flags |= NFSD4_SLOT_INUSE;
	if (seq->cachethis)
		slot->sl_flags |= NFSD4_SLOT_CACHETHIS;
	else
		slot->sl_flags &= ~NFSD4_SLOT_CACHETHIS;

	cstate->slot = slot;
	cstate->session = session;
	cstate->clp = clp;

out:
	switch (clp->cl_cb_state) {
	case NFSD4_CB_DOWN:
		seq->status_flags = SEQ4_STATUS_CB_PATH_DOWN;
		break;
	case NFSD4_CB_FAULT:
		seq->status_flags = SEQ4_STATUS_BACKCHANNEL_FAULT;
		break;
	default:
		seq->status_flags = 0;
	}
	if (!list_empty(&clp->cl_revoked))
		seq->status_flags |= SEQ4_STATUS_RECALLABLE_STATE_REVOKED;
out_no_session:
	if (conn)
		free_conn(conn);
	spin_unlock(&nn->client_lock);
	return status;
out_put_session:
	nfsd4_put_session_locked(session);
	goto out_no_session;
}