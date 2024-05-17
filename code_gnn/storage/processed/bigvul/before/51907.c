AirPDcapRsna4WHandshake(
    PAIRPDCAP_CONTEXT ctx,
    const UCHAR *data,
    AIRPDCAP_SEC_ASSOCIATION *sa,
    INT offset,
    const guint tot_len)
{
    AIRPDCAP_KEY_ITEM *tmp_key, *tmp_pkt_key, pkt_key;
    AIRPDCAP_SEC_ASSOCIATION *tmp_sa;
    INT key_index;
    INT ret_value=1;
    UCHAR useCache=FALSE;
    UCHAR eapol[AIRPDCAP_EAPOL_MAX_LEN];
    USHORT eapol_len;

    if (sa->key!=NULL)
        useCache=TRUE;

     
    if (AIRPDCAP_EAP_KEY(data[offset+1])!=1) {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "Group/STAKey message (not used)", AIRPDCAP_DEBUG_LEVEL_5);
        return AIRPDCAP_RET_NO_VALID_HANDSHAKE;
    }

     

     

     

    AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "4-way handshake...", AIRPDCAP_DEBUG_LEVEL_5);

     

     
    if (AIRPDCAP_EAP_INST(data[offset+1])==0 &&
        AIRPDCAP_EAP_ACK(data[offset+1])==1 &&
        AIRPDCAP_EAP_MIC(data[offset])==0)
    {
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "4-way handshake message 1", AIRPDCAP_DEBUG_LEVEL_3);

         
         
         
         

         
        if( sa->handshake >= 2) {
            tmp_sa= g_new(AIRPDCAP_SEC_ASSOCIATION, 1);
            memcpy(tmp_sa, sa, sizeof(AIRPDCAP_SEC_ASSOCIATION));
            sa->validKey=FALSE;
            sa->next=tmp_sa;
        }

         
        memcpy(sa->wpa.nonce, data+offset+12, 32);

         
        sa->wpa.key_ver=AIRPDCAP_EAP_KEY_DESCR_VER(data[offset+1]);

        sa->handshake=1;

        return AIRPDCAP_RET_SUCCESS_HANDSHAKE;
    }

     
    if (AIRPDCAP_EAP_INST(data[offset+1])==0 &&
        AIRPDCAP_EAP_ACK(data[offset+1])==0 &&
        AIRPDCAP_EAP_MIC(data[offset])==1)
    {
         
        if (pntoh16(data+offset+92)) {
             
            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "4-way handshake message 2", AIRPDCAP_DEBUG_LEVEL_3);

             
             
             
             
             

             
            for (key_index=0; key_index<(INT)ctx->keys_nr || useCache; key_index++) {
                 
                if (!useCache) {
                    AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "Try WPA key...", AIRPDCAP_DEBUG_LEVEL_3);
                    tmp_key=&ctx->keys[key_index];
                } else {
                     
                    if (sa->key!=NULL &&
                        (sa->key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PWD ||
                         sa->key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PSK ||
                         sa->key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PMK)) {
                            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "Try cached WPA key...", AIRPDCAP_DEBUG_LEVEL_3);
                            tmp_key=sa->key;
                    } else {
                        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "Cached key is of a wrong type, try WPA key...", AIRPDCAP_DEBUG_LEVEL_3);
                        tmp_key=&ctx->keys[key_index];
                    }
                }

                 
                if (tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PWD ||
                    tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PSK ||
                    tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PMK)
                {
                    if (tmp_key->KeyType == AIRPDCAP_KEY_TYPE_WPA_PWD && tmp_key->UserPwd.SsidLen == 0 && ctx->pkt_ssid_len > 0 && ctx->pkt_ssid_len <= AIRPDCAP_WPA_SSID_MAX_LEN) {
                         
                        memcpy(&pkt_key, tmp_key, sizeof(pkt_key));
                        memcpy(&pkt_key.UserPwd.Ssid, ctx->pkt_ssid, ctx->pkt_ssid_len);
                         pkt_key.UserPwd.SsidLen = ctx->pkt_ssid_len;
                        AirPDcapRsnaPwd2Psk(pkt_key.UserPwd.Passphrase, pkt_key.UserPwd.Ssid,
                            pkt_key.UserPwd.SsidLen, pkt_key.KeyData.Wpa.Psk);
                        tmp_pkt_key = &pkt_key;
                    } else {
                        tmp_pkt_key = tmp_key;
                    }

                     
                    AirPDcapRsnaPrfX(sa,                             
                                     tmp_pkt_key->KeyData.Wpa.Psk,       
                                     data+offset+12,                 
                                     512,
                                     sa->wpa.ptk);

                     
                    eapol_len=pntoh16(data+offset-3)+4;
                    memcpy(eapol, &data[offset-5], (eapol_len<AIRPDCAP_EAPOL_MAX_LEN?eapol_len:AIRPDCAP_EAPOL_MAX_LEN));
                    ret_value=AirPDcapRsnaMicCheck(eapol,            
                                                   eapol_len,        
                                                   sa->wpa.ptk,      
                                                   AIRPDCAP_EAP_KEY_DESCR_VER(data[offset+1]));  

                     
                     
                     
                     
                     
                }

                if (!ret_value &&
                    (tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PWD ||
                    tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PSK ||
                    tmp_key->KeyType==AIRPDCAP_KEY_TYPE_WPA_PMK))
                {
                     

                    sa->key=tmp_key;
                    break;
                } else {
                     

                    if (useCache==TRUE) {
                        useCache=FALSE;
                        key_index--;
                    }
                }
            }

            if (ret_value) {
                AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "handshake step failed", AIRPDCAP_DEBUG_LEVEL_3);
                return AIRPDCAP_RET_NO_VALID_HANDSHAKE;
            }

            sa->handshake=2;
            sa->validKey=TRUE;  

            return AIRPDCAP_RET_SUCCESS_HANDSHAKE;
        } else {
         

             

             
             
             
             
             

            AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "4-way handshake message 4", AIRPDCAP_DEBUG_LEVEL_3);

            sa->handshake=4;

            return AIRPDCAP_RET_SUCCESS_HANDSHAKE;
        }
    }

     
    if (AIRPDCAP_EAP_ACK(data[offset+1])==1 &&
        AIRPDCAP_EAP_MIC(data[offset])==1)
    {
        const EAPOL_RSN_KEY *pEAPKey;
        AIRPDCAP_DEBUG_PRINT_LINE("AirPDcapRsna4WHandshake", "4-way handshake message 3", AIRPDCAP_DEBUG_LEVEL_3);

         
         
         

         

         
        pEAPKey = (const EAPOL_RSN_KEY *)(&(data[offset-1]));
        if (pEAPKey->type == AIRPDCAP_RSN_WPA2_KEY_DESCRIPTOR){
            PAIRPDCAP_SEC_ASSOCIATION broadcast_sa;
            AIRPDCAP_SEC_ASSOCIATION_ID id;

             
            memcpy(id.sta, broadcast_mac, AIRPDCAP_MAC_LEN);
            memcpy(id.bssid, sa->saId.bssid, AIRPDCAP_MAC_LEN);
            broadcast_sa = AirPDcapGetSaPtr(ctx, &id);

            if (broadcast_sa == NULL){
                return AIRPDCAP_RET_REQ_DATA;
            }
            return (AirPDcapDecryptWPABroadcastKey(pEAPKey, sa->wpa.ptk+16, broadcast_sa, tot_len-offset+1));
        }
    }

    return AIRPDCAP_RET_NO_VALID_HANDSHAKE;
}
