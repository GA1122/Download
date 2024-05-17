static int decode_sequence(struct xdr_stream *xdr,
			   struct nfs4_sequence_res *res,
			   struct rpc_rqst *rqstp)
{
#if defined(CONFIG_NFS_V4_1)
	struct nfs4_sessionid id;
	u32 dummy;
	int status;
	__be32 *p;

	if (!res->sr_session)
		return 0;

	status = decode_op_hdr(xdr, OP_SEQUENCE);
	if (!status)
		status = decode_sessionid(xdr, &id);
	if (unlikely(status))
		goto out_err;

	 
	status = -EREMOTEIO;

	if (memcmp(id.data, res->sr_session->sess_id.data,
		   NFS4_MAX_SESSIONID_LEN)) {
		dprintk("%s Invalid session id\n", __func__);
		goto out_err;
	}

	p = xdr_inline_decode(xdr, 20);
	if (unlikely(!p))
		goto out_overflow;

	 
	dummy = be32_to_cpup(p++);
	if (dummy != res->sr_slot->seq_nr) {
		dprintk("%s Invalid sequence number\n", __func__);
		goto out_err;
	}
	 
	dummy = be32_to_cpup(p++);
	if (dummy != res->sr_slot - res->sr_session->fc_slot_table.slots) {
		dprintk("%s Invalid slot id\n", __func__);
		goto out_err;
	}
	 
	dummy = be32_to_cpup(p++);
	 
	dummy = be32_to_cpup(p++);
	 
	res->sr_status_flags = be32_to_cpup(p);
	status = 0;
out_err:
	res->sr_status = status;
	return status;
out_overflow:
	print_overflow_msg(__func__, xdr);
	status = -EIO;
	goto out_err;
#else   
	return 0;
#endif  
}
