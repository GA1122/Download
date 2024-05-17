ar6000_ready_event(void *devt, u8 *datap, u8 phyCap, u32 sw_ver, u32 abi_ver)
{
    struct ar6_softc *ar = (struct ar6_softc *)devt;
    struct net_device *dev = ar->arNetDev;

    memcpy(dev->dev_addr, datap, AR6000_ETH_ADDR_LEN);
    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("mac address = %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n",
        dev->dev_addr[0], dev->dev_addr[1],
        dev->dev_addr[2], dev->dev_addr[3],
        dev->dev_addr[4], dev->dev_addr[5]));

    ar->arPhyCapability = phyCap;
    ar->arVersion.wlan_ver = sw_ver;
    ar->arVersion.abi_ver = abi_ver;

     
    ar->arWmiReady = true;
    wake_up(&arEvent);
}