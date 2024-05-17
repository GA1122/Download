static void encode_create_session(struct xdr_stream *xdr,
				  struct nfs41_create_session_args *args,
				  struct compound_hdr *hdr)
{
	__be32 *p;
	char machine_name[NFS4_MAX_MACHINE_NAME_LEN];
	uint32_t len;
	struct nfs_client *clp = args->client;
	u32 max_resp_sz_cached;

	 
	max_resp_sz_cached = (NFS4_dec_open_sz + RPC_REPHDRSIZE +
			      RPC_MAX_AUTH_SIZE + 2) * XDR_UNIT;

	len = scnprintf(machine_name, sizeof(machine_name), "%s",
			clp->cl_ipaddr);

	p = reserve_space(xdr, 20 + 2*28 + 20 + len + 12);
	*p++ = cpu_to_be32(OP_CREATE_SESSION);
	p = xdr_encode_hyper(p, clp->cl_clientid);
	*p++ = cpu_to_be32(clp->cl_seqid);			 
	*p++ = cpu_to_be32(args->flags);			 

	 
	*p++ = cpu_to_be32(0);				 
	*p++ = cpu_to_be32(args->fc_attrs.max_rqst_sz);	 
	*p++ = cpu_to_be32(args->fc_attrs.max_resp_sz);	 
	*p++ = cpu_to_be32(max_resp_sz_cached);		 
	*p++ = cpu_to_be32(args->fc_attrs.max_ops);	 
	*p++ = cpu_to_be32(args->fc_attrs.max_reqs);	 
	*p++ = cpu_to_be32(0);				 

	 
	*p++ = cpu_to_be32(0);				 
	*p++ = cpu_to_be32(args->bc_attrs.max_rqst_sz);	 
	*p++ = cpu_to_be32(args->bc_attrs.max_resp_sz);	 
	*p++ = cpu_to_be32(args->bc_attrs.max_resp_sz_cached);	 
	*p++ = cpu_to_be32(args->bc_attrs.max_ops);	 
	*p++ = cpu_to_be32(args->bc_attrs.max_reqs);	 
	*p++ = cpu_to_be32(0);				 

	*p++ = cpu_to_be32(args->cb_program);		 
	*p++ = cpu_to_be32(1);
	*p++ = cpu_to_be32(RPC_AUTH_UNIX);			 

	 
	*p++ = cpu_to_be32((u32)clp->cl_boot_time.tv_nsec);	 
	p = xdr_encode_opaque(p, machine_name, len);
	*p++ = cpu_to_be32(0);				 
	*p++ = cpu_to_be32(0);				 
	*p = cpu_to_be32(0);				 
	hdr->nops++;
	hdr->replen += decode_create_session_maxsz;
}
