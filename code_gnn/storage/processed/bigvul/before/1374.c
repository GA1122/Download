static int rpc_check_reply(unsigned char *pkt, int isnfs)
{
	uint32_t *data;
	int nfserr;
	struct rpc_reply rpc;

	memcpy(&rpc, pkt, sizeof(rpc));

	if (ntohl(rpc.id) != rpc_id)
		return -EINVAL;

	if (rpc.rstatus  ||
	    rpc.verifier ||
	    rpc.astatus ) {
		return -EINVAL;
	}

	if (!isnfs)
		return 0;

	data = (uint32_t *)(pkt + sizeof(struct rpc_reply));
	nfserr = ntohl(net_read_uint32(data));

	debug("%s: state: %d, err %d\n", __func__, nfs_state, -nfserr);

	if (nfserr <= 30)
		 
		return -nfserr;
	if (nfserr == NFSERR_STALE)
		return -ESTALE;

	return -EINVAL;
}
