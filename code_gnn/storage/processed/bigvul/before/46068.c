xdr_krb5_enctype(XDR *xdrs, krb5_enctype *objp)
{
    

   if (!xdr_int32(xdrs, (int32_t *) objp))
	return (FALSE);
   return (TRUE);
}