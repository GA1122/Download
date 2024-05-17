bool_t xdr_krb5_key_data_nocontents(XDR *xdrs, krb5_key_data *objp)
{
      
     unsigned int tmp;

     if (xdrs->x_op == XDR_DECODE)
	  memset(objp, 0, sizeof(krb5_key_data));

     if (!xdr_krb5_int16(xdrs, &objp->key_data_ver)) {
	  return (FALSE);
     }
     if (!xdr_krb5_ui_2(xdrs, &objp->key_data_kvno)) {
	  return (FALSE);
     }
     if (!xdr_krb5_int16(xdrs, &objp->key_data_type[0])) {
	  return (FALSE);
     }
     if (objp->key_data_ver > 1) {
	  if (!xdr_krb5_int16(xdrs, &objp->key_data_type[1])) {
	       return (FALSE);
	  }
     }
      
     if (xdrs->x_op == XDR_FREE) {
	  tmp = (unsigned int) objp->key_data_length[0];
	  if (!xdr_bytes(xdrs, (char **) &objp->key_data_contents[0],
			 &tmp, ~0))
	       return FALSE;

	  tmp = (unsigned int) objp->key_data_length[1];
	  if (!xdr_bytes(xdrs, (char **) &objp->key_data_contents[1],
			 &tmp, ~0))
	       return FALSE;
     }

     return (TRUE);
}
