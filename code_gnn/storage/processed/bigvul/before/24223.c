int ar6000_start_ap_interface(struct ar6_softc *ar)
{
    struct ar_virtual_interface *arApDev;

     
    arApDev = (struct ar_virtual_interface *)ar->arApDev;
    ar->arNetDev = arApDev->arNetDev;

    return 0;
}
