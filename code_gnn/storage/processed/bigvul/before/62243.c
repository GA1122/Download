bgp_attr_get_as_size(netdissect_options *ndo,
                     uint8_t bgpa_type, const u_char *pptr, int len)
{
    const u_char *tptr = pptr;

     
    if (bgpa_type == BGPTYPE_AS4_PATH) {
        return 4;
    }

     
    while (tptr < pptr + len) {
        ND_TCHECK(tptr[0]);

         
        if (tptr[0] < BGP_AS_SEG_TYPE_MIN || tptr[0] > BGP_AS_SEG_TYPE_MAX) {
            goto trunc;
        }
        ND_TCHECK(tptr[1]);
        tptr += 2 + tptr[1] * 2;
    }

     
    if (tptr == pptr + len) {
        return 2;
    }

trunc:

     
   return 4;
}
