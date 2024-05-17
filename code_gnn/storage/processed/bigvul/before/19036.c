static int tcp_v4_md5_hash_pseudoheader(struct tcp_md5sig_pool *hp,
					__be32 daddr, __be32 saddr, int nbytes)
{
	struct tcp4_pseudohdr *bp;
	struct scatterlist sg;

	bp = &hp->md5_blk.ip4;

	 
	bp->saddr = saddr;
	bp->daddr = daddr;
	bp->pad = 0;
	bp->protocol = IPPROTO_TCP;
	bp->len = cpu_to_be16(nbytes);

	sg_init_one(&sg, bp, sizeof(*bp));
	return crypto_hash_update(&hp->md5_desc, &sg, sizeof(*bp));
}