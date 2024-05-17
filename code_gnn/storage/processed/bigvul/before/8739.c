vcard_apdu_set_length(VCardAPDU *apdu)
{
    int L, Le;

     
    L = apdu->a_len-4;  
    apdu->a_Lc = 0;
    apdu->a_Le = 0;
    apdu->a_body = NULL;
    switch (L) {
    case 0:
         
        return VCARD7816_STATUS_SUCCESS;
    case 1:
         
         
        apdu->a_Le = apdu->a_header->ah_Le ?
                         apdu->a_header->ah_Le : 256;
        return VCARD7816_STATUS_SUCCESS;
    default:
         
        if (apdu->a_header->ah_Le == 0) {
            if (L < 3) {
                 
                return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
            }
             
            Le = (apdu->a_header->ah_body[0] << 8)
                | apdu->a_header->ah_body[1];
            if (L == 3) {
                 
                 
                apdu->a_Le = Le ? Le : 65536;
                return VCARD7816_STATUS_SUCCESS;
            }
            if (Le == 0) {
                 
                return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
            }
             
            apdu->a_Lc = Le;
            apdu->a_body = &apdu->a_header->ah_body[2];
            if (L == Le+3) {
                 
                return VCARD7816_STATUS_SUCCESS;
            }
            if (L == Le+5) {
                 
                Le = (apdu->a_data[apdu->a_len-2] << 8)
                    | apdu->a_data[apdu->a_len-1];
                apdu->a_Le = Le ? Le : 65536;
                return VCARD7816_STATUS_SUCCESS;
            }
            return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
        }
         
        apdu->a_Lc = apdu->a_header->ah_Le;
        apdu->a_body = &apdu->a_header->ah_body[0];
        if (L ==  apdu->a_Lc + 1) {
             
            return VCARD7816_STATUS_SUCCESS;
        }
        if (L ==  apdu->a_Lc + 2) {
             
            Le = apdu->a_data[apdu->a_len-1];
            apdu->a_Le = Le ?  Le : 256;
            return VCARD7816_STATUS_SUCCESS;
        }
        break;
    }
    return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
}
