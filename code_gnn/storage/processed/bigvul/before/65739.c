nfsd4_decode_copy(struct nfsd4_compoundargs *argp, struct nfsd4_copy *copy)
{
	DECODE_HEAD;
	unsigned int tmp;

	status = nfsd4_decode_stateid(argp, &copy->cp_src_stateid);
	if (status)
		return status;
	status = nfsd4_decode_stateid(argp, &copy->cp_dst_stateid);
	if (status)
		return status;

	READ_BUF(8 + 8 + 8 + 4 + 4 + 4);
	p = xdr_decode_hyper(p, &copy->cp_src_pos);
	p = xdr_decode_hyper(p, &copy->cp_dst_pos);
	p = xdr_decode_hyper(p, &copy->cp_count);
	copy->cp_consecutive = be32_to_cpup(p++);
	copy->cp_synchronous = be32_to_cpup(p++);
	tmp = be32_to_cpup(p);  

	DECODE_TAIL;
}