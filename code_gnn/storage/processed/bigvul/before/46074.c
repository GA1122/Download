xdr_krb5_kvno(XDR *xdrs, krb5_kvno *objp)
{
	unsigned char tmp;

	tmp = '\0';  

	if (xdrs->x_op == XDR_ENCODE)
		tmp = (unsigned char) *objp;

	if (!xdr_u_char(xdrs, &tmp))
		return (FALSE);

	if (xdrs->x_op == XDR_DECODE)
		*objp = (krb5_kvno) tmp;

	return (TRUE);
}
