int ar6000_init(struct net_device *dev)
{
    struct ar6_softc *ar;
    int    status;
    s32 timeleft;
    s16 i;
    int         ret = 0;

    if((ar = ar6k_priv(dev)) == NULL)
    {
        return -EIO;
    }

    if (wlaninitmode == WLAN_INIT_MODE_USR || wlaninitmode == WLAN_INIT_MODE_DRV) {
    
        ar6000_update_bdaddr(ar);

        if (enablerssicompensation) {
            ar6000_copy_cust_data_from_target(ar->arHifDevice, ar->arTargetType);
            read_rssi_compensation_param(ar);
            for (i=-95; i<=0; i++) {
                rssi_compensation_table[0-i] = rssi_compensation_calc(ar,i);
            }
        }
    }

    dev_hold(dev);
    rtnl_unlock();

     
    if ((wlaninitmode == WLAN_INIT_MODE_USR || wlaninitmode == WLAN_INIT_MODE_DRV) && 
        (BMIDone(ar->arHifDevice) != 0))
    {
        ret = -EIO;
        goto ar6000_init_done;
    }

    if (!bypasswmi)
    {
#if 0  
        if (ar->arVersion.host_ver != ar->arVersion.target_ver) {
            A_PRINTF("WARNING: Host version 0x%x does not match Target "
                    " version 0x%x!\n",
                    ar->arVersion.host_ver, ar->arVersion.target_ver);
        }
#endif

         
        ar->arWmiEnabled = true;
        if ((ar->arWmi = wmi_init((void *) ar)) == NULL)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s() Failed to initialize WMI.\n", __func__));
            ret = -EIO;
            goto ar6000_init_done;
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s() Got WMI @ 0x%lx.\n", __func__,
            (unsigned long) ar->arWmi));
    }

    do {
        struct htc_service_connect_req connect;

             
        status = HTCWaitTarget(ar->arHtcTarget);

        if (status) {
            break;
        }

        A_MEMZERO(&connect,sizeof(connect));
             
        connect.pMetaData = NULL;
        connect.MetaDataLength = 0;
             
        connect.EpCallbacks.pContext = ar;
        connect.EpCallbacks.EpTxCompleteMultiple = ar6000_tx_complete;
        connect.EpCallbacks.EpRecv = ar6000_rx;
        connect.EpCallbacks.EpRecvRefill = ar6000_rx_refill;
        connect.EpCallbacks.EpSendFull = ar6000_tx_queue_full;
             
        connect.MaxSendQueueDepth = MAX_DEFAULT_SEND_QUEUE_DEPTH;
        connect.EpCallbacks.RecvRefillWaterMark = AR6000_MAX_RX_BUFFERS / 4;  
        if (0 == connect.EpCallbacks.RecvRefillWaterMark) {
            connect.EpCallbacks.RecvRefillWaterMark++;
        }
             
        connect.ServiceID = WMI_CONTROL_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI CONTROL");
        if (status) {
            break;
        }

        connect.LocalConnectionFlags |= HTC_LOCAL_CONN_FLAGS_ENABLE_SEND_BUNDLE_PADDING;
             
        connect.MaxSendMsgSize = WMI_MAX_TX_DATA_FRAME_LENGTH;

             
        connect.EpCallbacks.RecvAllocThreshold = AR6000_BUFFER_SIZE;
        connect.EpCallbacks.EpRecvAllocThresh = ar6000_alloc_amsdu_rxbuf;

             
        if (reduce_credit_dribble) {
            connect.ConnectionFlags |= HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE;
             
            connect.ConnectionFlags &= ~HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
            connect.ConnectionFlags |=
                        ((u16)reduce_credit_dribble - 1) & HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
        }
             
        connect.ServiceID = WMI_DATA_BE_SVC;

        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA BE");
        if (status) {
            break;
        }

             
        connect.ServiceID = WMI_DATA_BK_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA BK");
        if (status) {
            break;
        }

             
        connect.ServiceID = WMI_DATA_VI_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA VI");
        if (status) {
            break;
        }

             
        connect.ServiceID = WMI_DATA_VO_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA VO");
        if (status) {
            break;
        }

        A_ASSERT(arAc2EndpointID(ar,WMM_AC_BE) != 0);
        A_ASSERT(arAc2EndpointID(ar,WMM_AC_BK) != 0);
        A_ASSERT(arAc2EndpointID(ar,WMM_AC_VI) != 0);
        A_ASSERT(arAc2EndpointID(ar,WMM_AC_VO) != 0);

             
        ar->arAcStreamPriMap[WMM_AC_BK] = 0;  
        ar->arAcStreamPriMap[WMM_AC_BE] = 1;  
        ar->arAcStreamPriMap[WMM_AC_VI] = 2;  
        ar->arAcStreamPriMap[WMM_AC_VO] = 3;  

#ifdef EXPORT_HCI_BRIDGE_INTERFACE
        if (setuphci && (NULL != ar6kHciTransCallbacks.setupTransport)) {
            struct hci_transport_misc_handles hciHandles;

            hciHandles.netDevice = ar->arNetDev;
            hciHandles.hifDevice = ar->arHifDevice;
            hciHandles.htcHandle = ar->arHtcTarget;
            status = (int)(ar6kHciTransCallbacks.setupTransport(&hciHandles));
        }
#else
        if (setuphci) {
                 
            status = ar6000_setup_hci(ar);
        }
#endif

    } while (false);

    if (status) {
        ret = -EIO;
        goto ar6000_init_done;
    }

	if (regscanmode) {
		u32 param;

		if (BMIReadMemory(ar->arHifDevice,
				  HOST_INTEREST_ITEM_ADDRESS(ar,
							     hi_option_flag),
							     (u8 *)&param,
							     4) != 0) {
			AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
					("BMIReadMemory forsetting "
					 "regscanmode failed\n"));
			return A_ERROR;
		}

		if (regscanmode == 1)
			param |= HI_OPTION_SKIP_REG_SCAN;
		else if (regscanmode == 2)
			param |= HI_OPTION_INIT_REG_SCAN;

		if (BMIWriteMemory(ar->arHifDevice,
				   HOST_INTEREST_ITEM_ADDRESS(ar,
							      hi_option_flag),
							      (u8 *)&param,
							      4) != 0) {
			AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
					("BMIWriteMemory forsetting "
					"regscanmode failed\n"));
			return A_ERROR;
		}
		AR_DEBUG_PRINTF(ATH_DEBUG_INFO, ("Regulatory scan mode set\n"));
	}

     

    ar6000_rx_refill(ar, ar->arControlEp);
    ar6000_rx_refill(ar, arAc2EndpointID(ar,WMM_AC_BE));

     
    if (bypasswmi) {
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_BK));
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_VI));
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_VO));
    }

     
    ar6000_refill_amsdu_rxbufs(ar,AR6000_MAX_AMSDU_RX_BUFFERS);

         
    ar6000_setup_credit_dist(ar->arHtcTarget, &ar->arCreditStateInfo);

     
     
    ar6000_cookie_init(ar);

     
    status = HTCStart(ar->arHtcTarget);

    if (status) {
        if (ar->arWmiEnabled == true) {
            wmi_shutdown(ar->arWmi);
            ar->arWmiEnabled = false;
            ar->arWmi = NULL;
        }
        ar6000_cookie_cleanup(ar);
        ret = -EIO;
        goto ar6000_init_done;
    }

    if (!bypasswmi) {
         
        timeleft = wait_event_interruptible_timeout(arEvent,
            (ar->arWmiReady == true), wmitimeout * HZ);

        if (ar->arVersion.abi_ver != AR6K_ABI_VERSION) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ABI Version mismatch: Host(0x%x), Target(0x%x)\n", AR6K_ABI_VERSION, ar->arVersion.abi_ver));
#ifndef ATH6K_SKIP_ABI_VERSION_CHECK
            ret = -EIO;
            goto ar6000_init_done;
#endif  
        }

        if(!timeleft || signal_pending(current))
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("WMI is not ready or wait was interrupted\n"));
            ret = -EIO;
            goto ar6000_init_done;
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s() WMI is ready\n", __func__));

         
        if ((ar6000_set_host_app_area(ar)) != 0) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set the host app area\n"));
        }
        ar6000_target_config_wlan_params(ar);
    }

    ar->arNumDataEndPts = 1;

    if (bypasswmi) {
             
        dev->dev_addr[0] = 0x00;
        dev->dev_addr[1] = 0x01;
        dev->dev_addr[2] = 0x02;
        dev->dev_addr[3] = 0xAA;
        dev->dev_addr[4] = 0xBB;
        dev->dev_addr[5] = 0xCC;
    }

ar6000_init_done:
    rtnl_lock();
    dev_put(dev);

    return ret;
}
