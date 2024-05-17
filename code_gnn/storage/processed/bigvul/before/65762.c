nfsd4_decode_open(struct nfsd4_compoundargs *argp, struct nfsd4_open *open)
{
	DECODE_HEAD;
	u32 dummy;

	memset(open->op_bmval, 0, sizeof(open->op_bmval));
	open->op_iattr.ia_valid = 0;
	open->op_openowner = NULL;

	open->op_xdr_error = 0;
	 
	READ_BUF(4);
	open->op_seqid = be32_to_cpup(p++);
	 
	status = nfsd4_decode_share_access(argp, &open->op_share_access,
					   &open->op_deleg_want, &dummy);
	if (status)
		goto xdr_error;
	status = nfsd4_decode_share_deny(argp, &open->op_share_deny);
	if (status)
		goto xdr_error;
	READ_BUF(sizeof(clientid_t));
	COPYMEM(&open->op_clientid, sizeof(clientid_t));
	status = nfsd4_decode_opaque(argp, &open->op_owner);
	if (status)
		goto xdr_error;
	READ_BUF(4);
	open->op_create = be32_to_cpup(p++);
	switch (open->op_create) {
	case NFS4_OPEN_NOCREATE:
		break;
	case NFS4_OPEN_CREATE:
		current->fs->umask = 0;
		READ_BUF(4);
		open->op_createmode = be32_to_cpup(p++);
		switch (open->op_createmode) {
		case NFS4_CREATE_UNCHECKED:
		case NFS4_CREATE_GUARDED:
			status = nfsd4_decode_fattr(argp, open->op_bmval,
				&open->op_iattr, &open->op_acl, &open->op_label,
				&current->fs->umask);
			if (status)
				goto out;
			break;
		case NFS4_CREATE_EXCLUSIVE:
			READ_BUF(NFS4_VERIFIER_SIZE);
			COPYMEM(open->op_verf.data, NFS4_VERIFIER_SIZE);
			break;
		case NFS4_CREATE_EXCLUSIVE4_1:
			if (argp->minorversion < 1)
				goto xdr_error;
			READ_BUF(NFS4_VERIFIER_SIZE);
			COPYMEM(open->op_verf.data, NFS4_VERIFIER_SIZE);
			status = nfsd4_decode_fattr(argp, open->op_bmval,
				&open->op_iattr, &open->op_acl, &open->op_label,
				&current->fs->umask);
			if (status)
				goto out;
			break;
		default:
			goto xdr_error;
		}
		break;
	default:
		goto xdr_error;
	}

	 
	READ_BUF(4);
	open->op_claim_type = be32_to_cpup(p++);
	switch (open->op_claim_type) {
	case NFS4_OPEN_CLAIM_NULL:
	case NFS4_OPEN_CLAIM_DELEGATE_PREV:
		READ_BUF(4);
		open->op_fname.len = be32_to_cpup(p++);
		READ_BUF(open->op_fname.len);
		SAVEMEM(open->op_fname.data, open->op_fname.len);
		if ((status = check_filename(open->op_fname.data, open->op_fname.len)))
			return status;
		break;
	case NFS4_OPEN_CLAIM_PREVIOUS:
		READ_BUF(4);
		open->op_delegate_type = be32_to_cpup(p++);
		break;
	case NFS4_OPEN_CLAIM_DELEGATE_CUR:
		status = nfsd4_decode_stateid(argp, &open->op_delegate_stateid);
		if (status)
			return status;
		READ_BUF(4);
		open->op_fname.len = be32_to_cpup(p++);
		READ_BUF(open->op_fname.len);
		SAVEMEM(open->op_fname.data, open->op_fname.len);
		if ((status = check_filename(open->op_fname.data, open->op_fname.len)))
			return status;
		break;
	case NFS4_OPEN_CLAIM_FH:
	case NFS4_OPEN_CLAIM_DELEG_PREV_FH:
		if (argp->minorversion < 1)
			goto xdr_error;
		 
		break;
	case NFS4_OPEN_CLAIM_DELEG_CUR_FH:
		if (argp->minorversion < 1)
			goto xdr_error;
		status = nfsd4_decode_stateid(argp, &open->op_delegate_stateid);
		if (status)
			return status;
		break;
	default:
		goto xdr_error;
	}

	DECODE_TAIL;
}