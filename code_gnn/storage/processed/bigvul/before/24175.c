ar6000_destroy(struct net_device *dev, unsigned int unregister)
{
    struct ar6_softc *ar;

    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("+ar6000_destroy \n"));
    
    if((dev == NULL) || ((ar = ar6k_priv(dev)) == NULL))
    {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s(): Failed to get device structure.\n", __func__));
        return;
    }

    ar->bIsDestroyProgress = true;

    if (down_interruptible(&ar->arSem)) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s(): down_interruptible failed \n", __func__));
        return;
    }

    if (ar->arWlanPowerState != WLAN_POWER_STATE_CUT_PWR) {
         
        ar6000_stop_endpoint(dev, false, true);
    }

    ar->arWlanState = WLAN_DISABLED;
    if (ar->arHtcTarget != NULL) {
         
        HTCDestroy(ar->arHtcTarget);
    }
    if (ar->arHifDevice != NULL) {
         
        HIFReleaseDevice(ar->arHifDevice);
        HIFShutDownDevice(ar->arHifDevice);
    }
    aggr_module_destroy(ar->aggr_cntxt);

        
    ar6000_cookie_cleanup(ar);

         
    ar6000_cleanup_amsdu_rxbufs(ar);

    ar6000_sysfs_bmi_deinit(ar);

     
    BMICleanup();

     
    memset(tx_attempt, 0, sizeof(tx_attempt));
    memset(tx_post, 0, sizeof(tx_post));
    memset(tx_complete, 0, sizeof(tx_complete));

#ifdef HTC_RAW_INTERFACE
    if (ar->arRawHtc) {
        kfree(ar->arRawHtc);
        ar->arRawHtc = NULL;
    }
#endif 
     
    if (unregister && is_netdev_registered) {		
        unregister_netdev(dev);
        is_netdev_registered = 0;
    }
    free_netdev(dev);

    ar6k_cfg80211_deinit(ar);

#ifdef CONFIG_AP_VIRTUL_ADAPTER_SUPPORT
    ar6000_remove_ap_interface();
#endif  

    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("-ar6000_destroy \n"));
}
