xdr_krb5_flags(XDR *xdrs, krb5_flags *objp)
{
   
	if (!xdr_int32(xdrs, (int32_t *) objp)) {
		return (FALSE);
	}
	return (TRUE);
}
