ar6000_stop_endpoint(struct net_device *dev, bool keepprofile, bool getdbglogs)
{
    struct ar6_softc *ar = (struct ar6_softc *)ar6k_priv(dev);

     
    netif_stop_queue(dev);

     
    if (ar->arWmiReady == true)
    {
        if (!bypasswmi)
        {
            bool disconnectIssued;
 
            disconnectIssued = (ar->arConnected) || (ar->arConnectPending);
            ar6000_disconnect(ar);
            if (!keepprofile) {
                ar6000_init_profile_info(ar);
            }

            A_UNTIMEOUT(&ar->disconnect_timer);

            if (getdbglogs) {
                ar6000_dbglog_get_debug_logs(ar);
            }

            ar->arWmiReady  = false;
            wmi_shutdown(ar->arWmi);
            ar->arWmiEnabled = false;
            ar->arWmi = NULL;
             
            if (disconnectIssued) {
                if(ar->arNetworkType & AP_NETWORK) {
                    ar6000_disconnect_event(ar, DISCONNECT_CMD, bcast_mac, 0, NULL, 0);
                } else {
                    ar6000_disconnect_event(ar, DISCONNECT_CMD, ar->arBssid, 0, NULL, 0);
                }
            }
            ar->user_savedkeys_stat = USER_SAVEDKEYS_STAT_INIT;
            ar->user_key_ctrl      = 0;
        }

         AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("%s(): WMI stopped\n", __func__));
    }
    else
    {
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("%s(): WMI not ready 0x%lx 0x%lx\n",
            __func__, (unsigned long) ar, (unsigned long) ar->arWmi));

         
        if(ar->arWmiEnabled == true)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("%s(): Shut down WMI\n", __func__));
            wmi_shutdown(ar->arWmi);
            ar->arWmiEnabled = false;
            ar->arWmi = NULL;
        }
    }

    if (ar->arHtcTarget != NULL) {
#ifdef EXPORT_HCI_BRIDGE_INTERFACE
        if (NULL != ar6kHciTransCallbacks.cleanupTransport) {
            ar6kHciTransCallbacks.cleanupTransport(NULL);
        }
#else
        if (NULL != ar->exitCallback) {
            struct ar3k_config_info ar3kconfig;
            int status;

            A_MEMZERO(&ar3kconfig,sizeof(ar3kconfig));
            ar6000_set_default_ar3kconfig(ar, (void *)&ar3kconfig);
            status = ar->exitCallback(&ar3kconfig);
            if (0 != status) {
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Failed to reset AR3K baud rate! \n"));
            }
        }
        if (setuphci)
        	ar6000_cleanup_hci(ar);
#endif
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,(" Shutting down HTC .... \n"));
         
        HTCStop(ar->arHtcTarget);
    }

    if (resetok) {
         
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,(" Attempting to reset target on instance destroy.... \n"));
        if (ar->arHifDevice != NULL) {
            bool coldReset = (ar->arTargetType == TARGET_TYPE_AR6003) ? true: false;
            ar6000_reset_device(ar->arHifDevice, ar->arTargetType, true, coldReset);
        }
    } else {
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,(" Host does not want target reset. \n"));
    }
        
    ar6000_cookie_cleanup(ar);

     
    ar6000_cleanup_amsdu_rxbufs(ar);
}
