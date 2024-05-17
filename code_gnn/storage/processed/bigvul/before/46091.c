bool_t xdr_ui_4(XDR *xdrs, krb5_ui_4 *objp)
{
   
  return xdr_u_int32(xdrs, (uint32_t *) objp);
}
