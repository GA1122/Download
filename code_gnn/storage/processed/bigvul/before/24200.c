ar6000_neighborReport_event(struct ar6_softc *ar, int numAps, WMI_NEIGHBOR_INFO *info)
{
#if WIRELESS_EXT >= 18
    struct iw_pmkid_cand *pmkcand;
#else  
    static const char *tag = "PRE-AUTH";
    char buf[128];
#endif  

    union iwreq_data wrqu;
    int i;

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_SCAN,("AR6000 Neighbor Report Event\n"));
    for (i=0; i < numAps; info++, i++) {
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_SCAN,("bssid %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x ",
            info->bssid[0], info->bssid[1], info->bssid[2],
            info->bssid[3], info->bssid[4], info->bssid[5]));
        if (info->bssFlags & WMI_PREAUTH_CAPABLE_BSS) {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_SCAN,("preauth-cap"));
        }
        if (info->bssFlags & WMI_PMKID_VALID_BSS) {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_SCAN,(" pmkid-valid\n"));
            continue;            
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_SCAN,("\n"));
        A_MEMZERO(&wrqu, sizeof(wrqu));
#if WIRELESS_EXT >= 18
        pmkcand = A_MALLOC_NOWAIT(sizeof(struct iw_pmkid_cand));
        A_MEMZERO(pmkcand, sizeof(struct iw_pmkid_cand));
        pmkcand->index = i;
        pmkcand->flags = info->bssFlags;
        memcpy(pmkcand->bssid.sa_data, info->bssid, ATH_MAC_LEN);
        wrqu.data.length = sizeof(struct iw_pmkid_cand);
        wireless_send_event(ar->arNetDev, IWEVPMKIDCAND, &wrqu, (char *)pmkcand);
        kfree(pmkcand);
#else  
        snprintf(buf, sizeof(buf), "%s%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
                 tag,
                 info->bssid[0], info->bssid[1], info->bssid[2],
                 info->bssid[3], info->bssid[4], info->bssid[5],
                 i, info->bssFlags);
        wrqu.data.length = strlen(buf);
        wireless_send_event(ar->arNetDev, IWEVCUSTOM, &wrqu, buf);
#endif  
    }
}