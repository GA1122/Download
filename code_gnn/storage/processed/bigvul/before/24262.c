sta_cleanup(struct ar6_softc *ar, u8 i)
{
    struct sk_buff *skb;

     
    A_MUTEX_LOCK(&ar->sta_list[i].psqLock);
    while (!A_NETBUF_QUEUE_EMPTY(&ar->sta_list[i].psq)) {
        skb = A_NETBUF_DEQUEUE(&ar->sta_list[i].psq);
        A_NETBUF_FREE(skb);
    }
    A_MUTEX_UNLOCK(&ar->sta_list[i].psqLock);

     
    A_MEMZERO(&ar->arAPStats.sta[ar->sta_list[i].aid-1], sizeof(WMI_PER_STA_STAT));
    A_MEMZERO(&ar->sta_list[i].mac, ATH_MAC_LEN);
    A_MEMZERO(&ar->sta_list[i].wpa_ie, IEEE80211_MAX_IE);
    ar->sta_list[i].aid = 0;
    ar->sta_list[i].flags = 0;

    ar->sta_list_index = ar->sta_list_index & ~(1 << i);

}