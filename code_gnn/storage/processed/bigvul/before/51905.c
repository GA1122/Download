INT AirPDcapPacketProcess(
    PAIRPDCAP_CONTEXT ctx,
    const guint8 *data,
    const guint mac_header_len,
    const guint tot_len,
    UCHAR *decrypt_data,
    guint *decrypt_len,
    PAIRPDCAP_KEY_ITEM key,
    gboolean scanHandshake)
{
    AIRPDCAP_SEC_ASSOCIATION_ID id;
    UCHAR tmp_data[AIRPDCAP_MAX_CAPLEN];
    guint tmp_len;

#ifdef _DEBUG
#define MSGBUF_LEN 255
    CHAR msgbuf[MSGBUF_LEN];
#endif

    AIRPDCAP_DEBUG_TRACE_START("AirPDcapPacketProcess");

    if (ctx==NULL) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "NULL context", AIRPDCAP_DEBUG_LEVEL_5);
        AIRPDCAP_DEBUG_TRACE_END("AirPDcapPacketProcess");
        return AIRPDCAP_RET_REQ_DATA;
    }
    if (data==NULL || tot_len==0) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "NULL data or length=0", AIRPDCAP_DEBUG_LEVEL_5);
        AIRPDCAP_DEBUG_TRACE_END("AirPDcapPacketProcess");
        return AIRPDCAP_RET_REQ_DATA;
    }

     
    if (!((AIRPDCAP_TYPE(data[0])==AIRPDCAP_TYPE_DATA) ||
          (AIRPDCAP_TYPE(data[0])==AIRPDCAP_TYPE_MANAGEMENT &&
           (AIRPDCAP_SUBTYPE(data[0])==AIRPDCAP_SUBTYPE_DISASS ||
            AIRPDCAP_SUBTYPE(data[0])==AIRPDCAP_SUBTYPE_DEAUTHENTICATION ||
            AIRPDCAP_SUBTYPE(data[0])==AIRPDCAP_SUBTYPE_ACTION)))) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "not data nor robust mgmt packet", AIRPDCAP_DEBUG_LEVEL_5);
        return AIRPDCAP_RET_NO_DATA;
    }

     
    if (tot_len < (UINT)(mac_header_len+AIRPDCAP_CRYPTED_DATA_MINLEN)) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "minimum length violated", AIRPDCAP_DEBUG_LEVEL_5);
        return AIRPDCAP_RET_WRONG_DATA_SIZE;
    }

     
    if (tot_len > AIRPDCAP_MAX_CAPLEN) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "length too large", AIRPDCAP_DEBUG_LEVEL_3);
        return AIRPDCAP_RET_UNSUCCESS;
    }

     
    if (AirPDcapGetSaAddress((const AIRPDCAP_MAC_FRAME_ADDR4 *)(data), &id) != AIRPDCAP_RET_SUCCESS) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "STA/BSSID not found", AIRPDCAP_DEBUG_LEVEL_5);
        return AIRPDCAP_RET_REQ_DATA;
    }

     
    if (AIRPDCAP_WEP(data[1])==0) {
        if (scanHandshake) {
             
            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "Unencrypted data", AIRPDCAP_DEBUG_LEVEL_3);
            return (AirPDcapScanForKeys(ctx, data, mac_header_len, tot_len, id));
        }
        return AIRPDCAP_RET_NO_DATA_ENCRYPTED;
    } else {
        PAIRPDCAP_SEC_ASSOCIATION sa;
        int offset = 0;

         
        sa = AirPDcapGetSaPtr(ctx, &id);
        if (sa == NULL){
            return AIRPDCAP_RET_REQ_DATA;
        }

         
        offset = mac_header_len;

        if (decrypt_data==NULL) {
            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "no decrypt buffer, use local", AIRPDCAP_DEBUG_LEVEL_3);
            decrypt_data=tmp_data;
            decrypt_len=&tmp_len;
        }

         
        *decrypt_len = tot_len;
        memcpy(decrypt_data, data, *decrypt_len);

         
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "Encrypted data", AIRPDCAP_DEBUG_LEVEL_3);

         
         
         
         
        if (AIRPDCAP_EXTIV(data[offset+3])==0) {
            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "WEP encryption", AIRPDCAP_DEBUG_LEVEL_3);
            return AirPDcapWepMng(ctx, decrypt_data, mac_header_len, decrypt_len, key, sa, offset);
        } else {
            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "TKIP or CCMP encryption", AIRPDCAP_DEBUG_LEVEL_3);

             
            if (AIRPDCAP_KEY_INDEX(data[offset+3])>=1){

                AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", "The key index >= 1. This is encrypted with a group key.", AIRPDCAP_DEBUG_LEVEL_3);

                 
                memcpy(id.sta, broadcast_mac, AIRPDCAP_MAC_LEN);

#ifdef _DEBUG
                g_snprintf(msgbuf, MSGBUF_LEN, "ST_MAC: %2X.%2X.%2X.%2X.%2X.%2X\t", id.sta[0],id.sta[1],id.sta[2],id.sta[3],id.sta[4],id.sta[5]);
                AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapPacketProcess", msgbuf, AIRPDCAP_DEBUG_LEVEL_3);
#endif

                 
                sa = AirPDcapGetSaPtr(ctx, &id);
                if (sa == NULL)
                    return AIRPDCAP_RET_REQ_DATA;
            }

             
            if (AirPDcapRsnaMng(decrypt_data, mac_header_len, decrypt_len, key, sa, offset) == AIRPDCAP_RET_SUCCESS) {
                 
                if (scanHandshake) {
                    return (AirPDcapScanForKeys(ctx, decrypt_data, mac_header_len, *decrypt_len, id));
                } else {
                    return AIRPDCAP_RET_SUCCESS;
                }
            }
        }
    }
    return AIRPDCAP_RET_UNSUCCESS;
}
