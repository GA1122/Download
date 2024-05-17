ar6000_open(struct net_device *dev)
{
    unsigned long  flags;
    struct ar6_softc    *ar = (struct ar6_softc *)ar6k_priv(dev);

    spin_lock_irqsave(&ar->arLock, flags);

    if(ar->arWlanState == WLAN_DISABLED) {
        ar->arWlanState = WLAN_ENABLED;
    }

    if( ar->arConnected || bypasswmi) {
        netif_carrier_on(dev);
         
        netif_wake_queue(dev);
    }
    else
        netif_carrier_off(dev);

    spin_unlock_irqrestore(&ar->arLock, flags);
    return 0;
}