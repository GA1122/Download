fetch_asn1_field(unsigned char *astream, unsigned int level,
                 unsigned int field, krb5_data *data)
{
    unsigned char *estream;      
    int classes;                 
    unsigned int levels = 0;             
    int lastlevel = 1000;        
    int length;                  
    int tag;                     
    unsigned char savelen;       

    classes = -1;
     
    astream++;
    estream = astream;
    if ((length = asn1length(&astream)) < 0) {
        return(-1);
    }
    estream += length;
     
    while (astream < estream) {
        if (!asn1_id_constructed(*astream)) {
            return(-1);
        }
        if (asn1_id_class(*astream) == ASN1_CLASS_CTX) {
            if ((tag = (int)asn1_id_tag(*astream)) <= lastlevel) {
                levels++;
                classes = -1;
            }
            lastlevel = tag;
            if (levels == level) {
                 
                if (tag == (int)field) {
                     
                    astream++;
                    savelen = *astream;
                    if ((length = asn1length(&astream)) < 0) {
                        return(-1);
                    }
                    data->length = length;
                     
                    if ((savelen & 0xff) == 0x80) {
                        data->length -=2 ;
                    }
                    data->data = (char *)astream;
                    return(0);
                } else if (tag <= classes) {
                     
                    return(-1);
                } else {
                    classes = tag;
                }
            }
        }
         
        astream++;
        if ((length = asn1length(&astream)) < 0) {
            return(-1);
        }
        if (levels == level) {
            astream += length;
        }
    }
    return(-1);
}
