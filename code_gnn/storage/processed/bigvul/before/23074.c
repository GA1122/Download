static int encode_open(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
	encode_openhdr(xdr, arg);
	encode_opentype(xdr, arg);
	switch (arg->claim) {
		case NFS4_OPEN_CLAIM_NULL:
			encode_claim_null(xdr, arg->name);
			break;
		case NFS4_OPEN_CLAIM_PREVIOUS:
			encode_claim_previous(xdr, arg->u.delegation_type);
			break;
		case NFS4_OPEN_CLAIM_DELEGATE_CUR:
			encode_claim_delegate_cur(xdr, arg->name, &arg->u.delegation);
			break;
		default:
			BUG();
	}
	return 0;
}