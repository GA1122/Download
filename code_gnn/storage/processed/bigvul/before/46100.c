bool_t xdr_gss_buf(
     XDR *xdrs,
     gss_buffer_t buf)
{
      
     bool_t result;
      
     unsigned int length = buf->length;
     result = xdr_bytes(xdrs, (char **) &buf->value, &length,
			(xdrs->x_op == XDR_DECODE && buf->value == NULL)
			? (unsigned int) -1 : (unsigned int) buf->length);
     buf->length = length;
     return result;
}
