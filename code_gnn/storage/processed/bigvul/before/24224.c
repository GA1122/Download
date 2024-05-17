int ar6000_stop_ap_interface(struct ar6_softc *ar)
{
    struct ar_virtual_interface *arApDev;

     
    arApDev = (struct ar_virtual_interface *)ar->arApDev;
    if (arApDev) {
        ar->arNetDev = arApDev->arStaNetDev;
    }

    return 0;
}
