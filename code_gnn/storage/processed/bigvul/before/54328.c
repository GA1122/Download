asn1length(unsigned char **astream)
{
    int length;          
    int sublen;          
    int blen;            
    unsigned char *p;    

    if (**astream & 0x80) {
        blen = **astream & 0x7f;
        if (blen > 3) {
            return(-1);
        }
        for (++*astream, length = 0; blen; ++*astream, blen--) {
            length = (length << 8) | **astream;
        }
        if (length == 0) {
             
            p = *astream;
            p++;
            while (1) {
                 
                if ((sublen = asn1length(&p)) < 0) {
                    return(-1);
                }
                p += sublen;
                 
                if ((!*p++) && (!*p)) {
                    p++;
                    break;
                }
            }
            length = p - *astream;
        }
    } else {
        length = **astream;
        ++*astream;
    }
    return(length);
}
