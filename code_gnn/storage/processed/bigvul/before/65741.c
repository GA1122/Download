nfsd4_decode_create_session(struct nfsd4_compoundargs *argp,
			    struct nfsd4_create_session *sess)
{
	DECODE_HEAD;
	u32 dummy;

	READ_BUF(16);
	COPYMEM(&sess->clientid, 8);
	sess->seqid = be32_to_cpup(p++);
	sess->flags = be32_to_cpup(p++);

	 
	READ_BUF(28);
	dummy = be32_to_cpup(p++);  
	sess->fore_channel.maxreq_sz = be32_to_cpup(p++);
	sess->fore_channel.maxresp_sz = be32_to_cpup(p++);
	sess->fore_channel.maxresp_cached = be32_to_cpup(p++);
	sess->fore_channel.maxops = be32_to_cpup(p++);
	sess->fore_channel.maxreqs = be32_to_cpup(p++);
	sess->fore_channel.nr_rdma_attrs = be32_to_cpup(p++);
	if (sess->fore_channel.nr_rdma_attrs == 1) {
		READ_BUF(4);
		sess->fore_channel.rdma_attrs = be32_to_cpup(p++);
	} else if (sess->fore_channel.nr_rdma_attrs > 1) {
		dprintk("Too many fore channel attr bitmaps!\n");
		goto xdr_error;
	}

	 
	READ_BUF(28);
	dummy = be32_to_cpup(p++);  
	sess->back_channel.maxreq_sz = be32_to_cpup(p++);
	sess->back_channel.maxresp_sz = be32_to_cpup(p++);
	sess->back_channel.maxresp_cached = be32_to_cpup(p++);
	sess->back_channel.maxops = be32_to_cpup(p++);
	sess->back_channel.maxreqs = be32_to_cpup(p++);
	sess->back_channel.nr_rdma_attrs = be32_to_cpup(p++);
	if (sess->back_channel.nr_rdma_attrs == 1) {
		READ_BUF(4);
		sess->back_channel.rdma_attrs = be32_to_cpup(p++);
	} else if (sess->back_channel.nr_rdma_attrs > 1) {
		dprintk("Too many back channel attr bitmaps!\n");
		goto xdr_error;
	}

	READ_BUF(4);
	sess->callback_prog = be32_to_cpup(p++);
	nfsd4_decode_cb_sec(argp, &sess->cb_sec);
	DECODE_TAIL;
}