static int nfs_lookup_req(struct nfs_priv *npriv, struct nfs_fh *fh,
			  const char *filename, struct inode *inode)
{
	struct nfs_inode *ninode = nfsi(inode);
	uint32_t data[1024];
	uint32_t *p;
	int len;
	struct packet *nfs_packet;

	 

	p = &(data[0]);
	p = rpc_add_credentials(p);

	 
	p = nfs_add_fh3(p, fh);

	 
	p = nfs_add_filename(p, strlen(filename), filename);

	len = p - &(data[0]);

	nfs_packet = rpc_req(npriv, PROG_NFS, NFSPROC3_LOOKUP, data, len);
	if (IS_ERR(nfs_packet))
		return PTR_ERR(nfs_packet);

	p = (void *)nfs_packet->data + sizeof(struct rpc_reply) + 4;

	ninode->fh.size = ntoh32(net_read_uint32(p++));
	if (ninode->fh.size > NFS3_FHSIZE) {
		debug("%s: file handle too big: %u\n", __func__,
		      ninode->fh.size);
		return -EIO;
	}
	memcpy(ninode->fh.data, p, ninode->fh.size);
	p += DIV_ROUND_UP(ninode->fh.size, 4);

	nfs_read_post_op_attr(p, inode);

	free(nfs_packet);

	return 0;
}
