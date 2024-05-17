int ar6000_add_ap_interface(struct ar6_softc *ar, char *ap_ifname)
{
     
    if (ar->arApDev != NULL) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_add_ap_interface: interface already present \n"));
        return 0;
    }

    if (ar6000_create_ap_interface(ar, ap_ifname) != 0) {
        return A_ERROR;
    }

    A_PRINTF("Add AP interface %s \n",ap_ifname);

    return ar6000_start_ap_interface(ar);
}