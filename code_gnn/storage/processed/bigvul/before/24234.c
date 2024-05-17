ar6000_tkip_micerr_event(struct ar6_softc *ar, u8 keyid, bool ismcast)
{
    static const char *tag = "MLME-MICHAELMICFAILURE.indication";
    char buf[128];
    union iwreq_data wrqu;

     
    if (ar->arNetworkType == AP_NETWORK) {
        sta_t *s = ieee80211_find_conn_for_aid(ar, (keyid >> 2));
        if(!s){
            A_PRINTF("AP TKIP MIC error received from Invalid aid / STA not found =%d\n", keyid);
            return;
        }
        A_PRINTF("AP TKIP MIC error received from aid=%d\n", keyid);
        snprintf(buf,sizeof(buf), "%s addr=%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",
            tag, s->mac[0],s->mac[1],s->mac[2],s->mac[3],s->mac[4],s->mac[5]);
    } else {

    ar6k_cfg80211_tkip_micerr_event(ar, keyid, ismcast);

        A_PRINTF("AR6000 TKIP MIC error received for keyid %d %scast\n",
             keyid & 0x3, ismcast ? "multi": "uni");
        snprintf(buf, sizeof(buf), "%s(keyid=%d %sicast)", tag, keyid & 0x3,
             ismcast ? "mult" : "un");
    }

    memset(&wrqu, 0, sizeof(wrqu));
    wrqu.data.length = strlen(buf);
    wireless_send_event(ar->arNetDev, IWEVCUSTOM, &wrqu, buf);
}