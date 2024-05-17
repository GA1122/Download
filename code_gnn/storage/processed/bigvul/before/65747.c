nfsd4_decode_fattr(struct nfsd4_compoundargs *argp, u32 *bmval,
		   struct iattr *iattr, struct nfs4_acl **acl,
		   struct xdr_netobj *label, int *umask)
{
	int expected_len, len = 0;
	u32 dummy32;
	char *buf;

	DECODE_HEAD;
	iattr->ia_valid = 0;
	if ((status = nfsd4_decode_bitmap(argp, bmval)))
		return status;

	if (bmval[0] & ~NFSD_WRITEABLE_ATTRS_WORD0
	    || bmval[1] & ~NFSD_WRITEABLE_ATTRS_WORD1
	    || bmval[2] & ~NFSD_WRITEABLE_ATTRS_WORD2) {
		if (nfsd_attrs_supported(argp->minorversion, bmval))
			return nfserr_inval;
		return nfserr_attrnotsupp;
	}

	READ_BUF(4);
	expected_len = be32_to_cpup(p++);

	if (bmval[0] & FATTR4_WORD0_SIZE) {
		READ_BUF(8);
		len += 8;
		p = xdr_decode_hyper(p, &iattr->ia_size);
		iattr->ia_valid |= ATTR_SIZE;
	}
	if (bmval[0] & FATTR4_WORD0_ACL) {
		u32 nace;
		struct nfs4_ace *ace;

		READ_BUF(4); len += 4;
		nace = be32_to_cpup(p++);

		if (nace > NFS4_ACL_MAX)
			return nfserr_fbig;

		*acl = svcxdr_tmpalloc(argp, nfs4_acl_bytes(nace));
		if (*acl == NULL)
			return nfserr_jukebox;

		(*acl)->naces = nace;
		for (ace = (*acl)->aces; ace < (*acl)->aces + nace; ace++) {
			READ_BUF(16); len += 16;
			ace->type = be32_to_cpup(p++);
			ace->flag = be32_to_cpup(p++);
			ace->access_mask = be32_to_cpup(p++);
			dummy32 = be32_to_cpup(p++);
			READ_BUF(dummy32);
			len += XDR_QUADLEN(dummy32) << 2;
			READMEM(buf, dummy32);
			ace->whotype = nfs4_acl_get_whotype(buf, dummy32);
			status = nfs_ok;
			if (ace->whotype != NFS4_ACL_WHO_NAMED)
				;
			else if (ace->flag & NFS4_ACE_IDENTIFIER_GROUP)
				status = nfsd_map_name_to_gid(argp->rqstp,
						buf, dummy32, &ace->who_gid);
			else
				status = nfsd_map_name_to_uid(argp->rqstp,
						buf, dummy32, &ace->who_uid);
			if (status)
				return status;
		}
	} else
		*acl = NULL;
	if (bmval[1] & FATTR4_WORD1_MODE) {
		READ_BUF(4);
		len += 4;
		iattr->ia_mode = be32_to_cpup(p++);
		iattr->ia_mode &= (S_IFMT | S_IALLUGO);
		iattr->ia_valid |= ATTR_MODE;
	}
	if (bmval[1] & FATTR4_WORD1_OWNER) {
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);
		READ_BUF(dummy32);
		len += (XDR_QUADLEN(dummy32) << 2);
		READMEM(buf, dummy32);
		if ((status = nfsd_map_name_to_uid(argp->rqstp, buf, dummy32, &iattr->ia_uid)))
			return status;
		iattr->ia_valid |= ATTR_UID;
	}
	if (bmval[1] & FATTR4_WORD1_OWNER_GROUP) {
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);
		READ_BUF(dummy32);
		len += (XDR_QUADLEN(dummy32) << 2);
		READMEM(buf, dummy32);
		if ((status = nfsd_map_name_to_gid(argp->rqstp, buf, dummy32, &iattr->ia_gid)))
			return status;
		iattr->ia_valid |= ATTR_GID;
	}
	if (bmval[1] & FATTR4_WORD1_TIME_ACCESS_SET) {
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);
		switch (dummy32) {
		case NFS4_SET_TO_CLIENT_TIME:
			len += 12;
			status = nfsd4_decode_time(argp, &iattr->ia_atime);
			if (status)
				return status;
			iattr->ia_valid |= (ATTR_ATIME | ATTR_ATIME_SET);
			break;
		case NFS4_SET_TO_SERVER_TIME:
			iattr->ia_valid |= ATTR_ATIME;
			break;
		default:
			goto xdr_error;
		}
	}
	if (bmval[1] & FATTR4_WORD1_TIME_MODIFY_SET) {
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);
		switch (dummy32) {
		case NFS4_SET_TO_CLIENT_TIME:
			len += 12;
			status = nfsd4_decode_time(argp, &iattr->ia_mtime);
			if (status)
				return status;
			iattr->ia_valid |= (ATTR_MTIME | ATTR_MTIME_SET);
			break;
		case NFS4_SET_TO_SERVER_TIME:
			iattr->ia_valid |= ATTR_MTIME;
			break;
		default:
			goto xdr_error;
		}
	}

	label->len = 0;
#ifdef CONFIG_NFSD_V4_SECURITY_LABEL
	if (bmval[2] & FATTR4_WORD2_SECURITY_LABEL) {
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);  
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);  
		READ_BUF(4);
		len += 4;
		dummy32 = be32_to_cpup(p++);
		READ_BUF(dummy32);
		if (dummy32 > NFS4_MAXLABELLEN)
			return nfserr_badlabel;
		len += (XDR_QUADLEN(dummy32) << 2);
		READMEM(buf, dummy32);
		label->len = dummy32;
		label->data = svcxdr_dupstr(argp, buf, dummy32);
		if (!label->data)
			return nfserr_jukebox;
	}
#endif
	if (bmval[2] & FATTR4_WORD2_MODE_UMASK) {
		if (!umask)
			goto xdr_error;
		READ_BUF(8);
		len += 8;
		dummy32 = be32_to_cpup(p++);
		iattr->ia_mode = dummy32 & (S_IFMT | S_IALLUGO);
		dummy32 = be32_to_cpup(p++);
		*umask = dummy32 & S_IRWXUGO;
		iattr->ia_valid |= ATTR_MODE;
	}
	if (len != expected_len)
		goto xdr_error;

	DECODE_TAIL;
}