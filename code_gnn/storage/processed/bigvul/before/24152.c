ar6000_avail_ev(void *context, void *hif_handle)
{
    int i;
    struct net_device *dev;
    void *ar_netif;
    struct ar6_softc *ar;
    int device_index = 0;
    struct htc_init_info  htcInfo;
    struct wireless_dev *wdev;
    int r = 0;
    struct hif_device_os_device_info osDevInfo;

    memset(&osDevInfo, 0, sizeof(osDevInfo));
    if (HIFConfigureDevice(hif_handle, HIF_DEVICE_GET_OS_DEVICE,
        &osDevInfo, sizeof(osDevInfo))) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s: Failed to get OS device instance\n", __func__));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("ar6000_available\n"));

    for (i=0; i < MAX_AR6000; i++) {
        if (ar6000_devices[i] == NULL) {
            break;
        }
    }

    if (i == MAX_AR6000) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_available: max devices reached\n"));
        return A_ERROR;
    }

     
     
    device_index = i;

    wdev = ar6k_cfg80211_init(osDevInfo.pOSDevice);
    if (IS_ERR(wdev)) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("%s: ar6k_cfg80211_init failed\n", __func__));
        return A_ERROR;
    }
    ar_netif = wdev_priv(wdev);

    if (ar_netif == NULL) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("%s: Can't allocate ar6k priv memory\n", __func__));
        return A_ERROR;
    }

    A_MEMZERO(ar_netif, sizeof(struct ar6_softc));
    ar = (struct ar6_softc *)ar_netif;

    ar->wdev = wdev;
    wdev->iftype = NL80211_IFTYPE_STATION;

    dev = alloc_netdev_mq(0, "wlan%d", ether_setup, 1);
    if (!dev) {
        printk(KERN_CRIT "AR6K: no memory for network device instance\n");
        ar6k_cfg80211_deinit(ar);
        return A_ERROR;
    }

    dev->ieee80211_ptr = wdev;
    SET_NETDEV_DEV(dev, wiphy_dev(wdev->wiphy));
    wdev->netdev = dev;
    ar->arNetworkType = INFRA_NETWORK;
    ar->smeState = SME_DISCONNECTED;
    ar->arAutoAuthStage = AUTH_IDLE;

    init_netdev(dev, ifname);


    ar->arNetDev             = dev;
    ar->arHifDevice          = hif_handle;
    ar->arWlanState          = WLAN_ENABLED;
    ar->arDeviceIndex        = device_index;

    ar->arWlanPowerState     = WLAN_POWER_STATE_ON;
    ar->arWlanOff            = false;    
#ifdef CONFIG_PM
    ar->arWowState           = WLAN_WOW_STATE_NONE;
    ar->arBTOff              = true;    
    ar->arBTSharing          = WLAN_CONFIG_BT_SHARING; 
    ar->arWlanOffConfig      = WLAN_CONFIG_WLAN_OFF;
    ar->arSuspendConfig      = WLAN_CONFIG_PM_SUSPEND;
    ar->arWow2Config         = WLAN_CONFIG_PM_WOW2;
#endif  

    A_INIT_TIMER(&ar->arHBChallengeResp.timer, ar6000_detect_error, dev);
    ar->arHBChallengeResp.seqNum = 0;
    ar->arHBChallengeResp.outstanding = false;
    ar->arHBChallengeResp.missCnt = 0;
    ar->arHBChallengeResp.frequency = AR6000_HB_CHALLENGE_RESP_FREQ_DEFAULT;
    ar->arHBChallengeResp.missThres = AR6000_HB_CHALLENGE_RESP_MISS_THRES_DEFAULT;

    ar6000_init_control_info(ar);
    init_waitqueue_head(&arEvent);
    sema_init(&ar->arSem, 1);
    ar->bIsDestroyProgress = false;

    INIT_HTC_PACKET_QUEUE(&ar->amsdu_rx_buffer_queue);

#ifdef ADAPTIVE_POWER_THROUGHPUT_CONTROL
    A_INIT_TIMER(&aptcTimer, aptcTimerHandler, ar);
#endif  

    A_INIT_TIMER(&ar->disconnect_timer, disconnect_timer_handler, dev);

    BMIInit();

    ar6000_sysfs_bmi_init(ar);

    {
        struct bmi_target_info targ_info;

        r = BMIGetTargetInfo(ar->arHifDevice, &targ_info);
        if (r)
            goto avail_ev_failed;

        ar->arVersion.target_ver = targ_info.target_ver;
        ar->arTargetType = targ_info.target_type;
	wdev->wiphy->hw_version = targ_info.target_ver;
    }

    r = ar6000_configure_target(ar);
    if (r)
            goto avail_ev_failed;

    A_MEMZERO(&htcInfo,sizeof(htcInfo));
    htcInfo.pContext = ar;
    htcInfo.TargetFailure = ar6000_target_failure;

    ar->arHtcTarget = HTCCreate(ar->arHifDevice,&htcInfo);

    if (!ar->arHtcTarget) {
        r = -ENOMEM;
        goto avail_ev_failed;
    }

    spin_lock_init(&ar->arLock);

#ifdef WAPI_ENABLE
    ar->arWapiEnable = 0;
#endif


    if(csumOffload){
         
        ar->rxMetaVersion=WMI_META_VERSION_2;
    }

    ar->aggr_cntxt = aggr_init(ar6000_alloc_netbufs);
    if (!ar->aggr_cntxt) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s() Failed to initialize aggr.\n", __func__));
            r = -ENOMEM;
            goto avail_ev_failed;
    }

    aggr_register_rx_dispatcher(ar->aggr_cntxt, (void *)dev, ar6000_deliver_frames_to_nw_stack);

    HIFClaimDevice(ar->arHifDevice, ar);

     
     
     
    ar6000_devices[device_index] = dev;

    AR_DEBUG_PRINTF(ATH_DEBUG_INFO, ("BMI enabled: %d\n", wlaninitmode));
    if ((wlaninitmode == WLAN_INIT_MODE_UDEV) ||
        (wlaninitmode == WLAN_INIT_MODE_DRV)) {
	r = ath6kl_init_netdev(ar);
	if (r)
            goto avail_ev_failed;
    }

     
    r = register_netdev(dev);
    if (r) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_avail: register_netdev failed\n"));
        ar6000_destroy(dev, 0);
        return r;
    }

	is_netdev_registered = 1;

#ifdef CONFIG_AP_VIRTUAL_ADAPTER_SUPPORT
    arApNetDev = NULL;
#endif  
    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("ar6000_avail: name=%s hifdevice=0x%lx, dev=0x%lx (%d), ar=0x%lx\n",
                    dev->name, (unsigned long)ar->arHifDevice, (unsigned long)dev, device_index,
                    (unsigned long)ar));

avail_ev_failed :
    if (r)
        ar6000_sysfs_bmi_deinit(ar);  

    return r;
}