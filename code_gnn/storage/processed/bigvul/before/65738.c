nfsd4_decode_compound(struct nfsd4_compoundargs *argp)
{
	DECODE_HEAD;
	struct nfsd4_op *op;
	bool cachethis = false;
	int auth_slack= argp->rqstp->rq_auth_slack;
	int max_reply = auth_slack + 8;  
	int readcount = 0;
	int readbytes = 0;
	int i;

	READ_BUF(4);
	argp->taglen = be32_to_cpup(p++);
	READ_BUF(argp->taglen);
	SAVEMEM(argp->tag, argp->taglen);
	READ_BUF(8);
	argp->minorversion = be32_to_cpup(p++);
	argp->opcnt = be32_to_cpup(p++);
	max_reply += 4 + (XDR_QUADLEN(argp->taglen) << 2);

	if (argp->taglen > NFSD4_MAX_TAGLEN)
		goto xdr_error;
	if (argp->opcnt > 100)
		goto xdr_error;

	if (argp->opcnt > ARRAY_SIZE(argp->iops)) {
		argp->ops = kzalloc(argp->opcnt * sizeof(*argp->ops), GFP_KERNEL);
		if (!argp->ops) {
			argp->ops = argp->iops;
			dprintk("nfsd: couldn't allocate room for COMPOUND\n");
			goto xdr_error;
		}
	}

	if (argp->minorversion > NFSD_SUPPORTED_MINOR_VERSION)
		argp->opcnt = 0;

	for (i = 0; i < argp->opcnt; i++) {
		op = &argp->ops[i];
		op->replay = NULL;

		READ_BUF(4);
		op->opnum = be32_to_cpup(p++);

		if (nfsd4_opnum_in_range(argp, op))
			op->status = nfsd4_dec_ops[op->opnum](argp, &op->u);
		else {
			op->opnum = OP_ILLEGAL;
			op->status = nfserr_op_illegal;
		}
		 
		cachethis |= nfsd4_cache_this_op(op);

		if (op->opnum == OP_READ) {
			readcount++;
			readbytes += nfsd4_max_reply(argp->rqstp, op);
		} else
			max_reply += nfsd4_max_reply(argp->rqstp, op);
		 
		if (op->opnum == OP_LOCK || op->opnum == OP_LOCKT)
			max_reply += NFS4_OPAQUE_LIMIT;

		if (op->status) {
			argp->opcnt = i+1;
			break;
		}
	}
	 
	if (argp->minorversion)
		cachethis = false;
	svc_reserve(argp->rqstp, max_reply + readbytes);
	argp->rqstp->rq_cachetype = cachethis ? RC_REPLBUFF : RC_NOCACHE;

	if (readcount > 1 || max_reply > PAGE_SIZE - auth_slack)
		clear_bit(RQ_SPLICE_OK, &argp->rqstp->rq_flags);

	DECODE_TAIL;
}