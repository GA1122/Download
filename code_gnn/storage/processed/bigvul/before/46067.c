xdr_krb5_deltat(XDR *xdrs, krb5_deltat *objp)
{
   
	if (!xdr_int32(xdrs, (int32_t *) objp)) {
		return (FALSE);
	}
	return (TRUE);
}
