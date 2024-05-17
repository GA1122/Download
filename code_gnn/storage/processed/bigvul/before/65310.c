static bool need_wrongsec_check(struct svc_rqst *rqstp)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
	struct nfsd4_compoundargs *argp = rqstp->rq_argp;
	struct nfsd4_op *this = &argp->ops[resp->opcnt - 1];
	struct nfsd4_op *next = &argp->ops[resp->opcnt];
	struct nfsd4_operation *thisd;
	struct nfsd4_operation *nextd;

	thisd = OPDESC(this);
	 
	if (!(thisd->op_flags & OP_IS_PUTFH_LIKE))
		return false;
	 
	if (argp->opcnt == resp->opcnt)
		return false;
	if (next->opnum == OP_ILLEGAL)
		return false;
	nextd = OPDESC(next);
	 
	return !(nextd->op_flags & OP_HANDLES_WRONGSEC);
}
