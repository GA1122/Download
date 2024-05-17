u8 ar6000_ibss_map_epid(struct sk_buff *skb, struct net_device *dev, u32 *mapNo)
{
    struct ar6_softc      *ar = (struct ar6_softc *)ar6k_priv(dev);
    u8 *datap;
    ATH_MAC_HDR     *macHdr;
    u32 i, eptMap;

    (*mapNo) = 0;
    datap = A_NETBUF_DATA(skb);
    macHdr = (ATH_MAC_HDR *)(datap + sizeof(WMI_DATA_HDR));
    if (IEEE80211_IS_MULTICAST(macHdr->dstMac)) {
        return ENDPOINT_2;
    }

    eptMap = -1;
    for (i = 0; i < ar->arNodeNum; i ++) {
        if (IEEE80211_ADDR_EQ(macHdr->dstMac, ar->arNodeMap[i].macAddress)) {
            (*mapNo) = i + 1;
            ar->arNodeMap[i].txPending ++;
            return ar->arNodeMap[i].epId;
        }

        if ((eptMap == -1) && !ar->arNodeMap[i].txPending) {
            eptMap = i;
        }
    }

    if (eptMap == -1) {
        eptMap = ar->arNodeNum;
        ar->arNodeNum ++;
        A_ASSERT(ar->arNodeNum <= MAX_NODE_NUM);
    }

    memcpy(ar->arNodeMap[eptMap].macAddress, macHdr->dstMac, IEEE80211_ADDR_LEN);

    for (i = ENDPOINT_2; i <= ENDPOINT_5; i ++) {
        if (!ar->arTxPending[i]) {
            ar->arNodeMap[eptMap].epId = i;
            break;
        }
        if (i == ENDPOINT_5) {
            ar->arNodeMap[eptMap].epId = ar->arNexEpId;
            ar->arNexEpId ++;
            if (ar->arNexEpId > ENDPOINT_5) {
                ar->arNexEpId = ENDPOINT_2;
            }
        }
    }

    (*mapNo) = eptMap + 1;
    ar->arNodeMap[eptMap].txPending ++;

    return ar->arNodeMap[eptMap].epId;
}