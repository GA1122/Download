static inline u32 nfsd4_copy_rsize(struct svc_rqst *rqstp, struct nfsd4_op *op)
{
	return (op_encode_hdr_size +
		1   +
		op_encode_stateid_maxsz   +
		2   +
		1   +
		op_encode_verifier_maxsz +
		1   +
		1  ) * sizeof(__be32);
}
