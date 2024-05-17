static void *nfs_readdirattr_req(struct nfs_priv *npriv, struct nfs_dir *dir)
{
	uint32_t data[1024];
	uint32_t *p;
	int len;
	struct packet *nfs_packet;
	void *buf;

	 

	p = &(data[0]);
	p = rpc_add_credentials(p);

	p = nfs_add_fh3(p, &dir->fh);
	p = nfs_add_uint64(p, dir->cookie);

	memcpy(p, dir->cookieverf, NFS3_COOKIEVERFSIZE);
	p += NFS3_COOKIEVERFSIZE / 4;

	p = nfs_add_uint32(p, 1024);  

	nfs_packet = rpc_req(npriv, PROG_NFS, NFSPROC3_READDIR, data, p - data);
	if (IS_ERR(nfs_packet))
		return NULL;

	p = (void *)nfs_packet->data + sizeof(struct rpc_reply) + 4;
	p = nfs_read_post_op_attr(p, NULL);

	 
	memcpy(dir->cookieverf, p, NFS3_COOKIEVERFSIZE);
	p += NFS3_COOKIEVERFSIZE / 4;

	len = (void *)nfs_packet->data + nfs_packet->len - (void *)p;
	if (!len) {
		printf("%s: huh, no payload left\n", __func__);
		free(nfs_packet);
		return NULL;
	}

	buf = xzalloc(len);

	memcpy(buf, p, len);

	free(nfs_packet);

	xdr_init(&dir->stream, buf, len);

	 

	return buf;
}
