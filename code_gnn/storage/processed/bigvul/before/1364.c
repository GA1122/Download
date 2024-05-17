static int nfs_lookup_reply(unsigned char *pkt, unsigned len)
{
	int ret;

	ret = rpc_check_reply(pkt, 1);
	if (ret)
		return ret;

	memcpy(filefh, pkt + sizeof(struct rpc_reply) + 4, NFS_FHSIZE);

	return 0;
}
