init_netdev(struct net_device *dev, char *name)
{
    dev->netdev_ops = &ar6000_netdev_ops;
    dev->watchdog_timeo = AR6000_TX_TIMEOUT;

    
    if (processDot11Hdr) {
        dev->hard_header_len = sizeof(struct ieee80211_qosframe) + sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR) + HTC_HEADER_LEN + WMI_MAX_TX_META_SZ + LINUX_HACK_FUDGE_FACTOR;
    } else {
        dev->hard_header_len = ETH_HLEN + sizeof(ATH_LLC_SNAP_HDR) +
            sizeof(WMI_DATA_HDR) + HTC_HEADER_LEN + WMI_MAX_TX_META_SZ + LINUX_HACK_FUDGE_FACTOR;
    }

    if (name[0])
    {
        strcpy(dev->name, name);
    }

#ifdef CONFIG_CHECKSUM_OFFLOAD
    if(csumOffload){
        dev->features |= NETIF_F_IP_CSUM;  
    }
#endif

    return;
}
