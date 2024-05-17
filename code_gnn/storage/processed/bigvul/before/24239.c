ar6000_unavail_ev(void *context, void *hif_handle)
{
    struct ar6_softc *ar = (struct ar6_softc *)context;
         
    ar6000_devices[ar->arDeviceIndex] = NULL;
    ar6000_destroy(ar->arNetDev, 1);

    return 0;
}