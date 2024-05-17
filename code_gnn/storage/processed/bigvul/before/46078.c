xdr_krb5_timestamp(XDR *xdrs, krb5_timestamp *objp)
{
   
	if (!xdr_int32(xdrs, (int32_t *) objp)) {
		return (FALSE);
	}
	return (TRUE);
}
