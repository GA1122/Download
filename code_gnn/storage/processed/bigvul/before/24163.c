ar6000_configure_target(struct ar6_softc *ar)
{
    u32 param;
    if (enableuartprint) {
        param = 1;
        if (BMIWriteMemory(ar->arHifDevice,
                           HOST_INTEREST_ITEM_ADDRESS(ar, hi_serial_enable),
                           (u8 *)&param,
                           4)!= 0)
        {
             AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIWriteMemory for enableuartprint failed \n"));
             return A_ERROR;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("Serial console prints enabled\n"));
    }

     
    param = HTC_PROTOCOL_VERSION;
    if (BMIWriteMemory(ar->arHifDevice,
                       HOST_INTEREST_ITEM_ADDRESS(ar, hi_app_host_interest),
                       (u8 *)&param,
                       4)!= 0)
    {
         AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIWriteMemory for htc version failed \n"));
         return A_ERROR;
    }

#ifdef CONFIG_HOST_TCMD_SUPPORT
    if(testmode) {
        ar->arTargetMode = AR6000_TCMD_MODE;
    }else {
        ar->arTargetMode = AR6000_WLAN_MODE;
    }
#endif
    if (enabletimerwar) {
        u32 param;

        if (BMIReadMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4)!= 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIReadMemory for enabletimerwar failed \n"));
            return A_ERROR;
        }

        param |= HI_OPTION_TIMER_WAR;

        if (BMIWriteMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4) != 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIWriteMemory for enabletimerwar failed \n"));
            return A_ERROR;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("Timer WAR enabled\n"));
    }

     
    {
        u32 param;

        if (BMIReadMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4)!= 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIReadMemory for setting fwmode failed \n"));
            return A_ERROR;
        }

        param |= (num_device << HI_OPTION_NUM_DEV_SHIFT);
        param |= (fwmode << HI_OPTION_FW_MODE_SHIFT);
        param |= (mac_addr_method << HI_OPTION_MAC_ADDR_METHOD_SHIFT);
        param |= (firmware_bridge << HI_OPTION_FW_BRIDGE_SHIFT);


        if (BMIWriteMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4) != 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIWriteMemory for setting fwmode failed \n"));
            return A_ERROR;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("Firmware mode set\n"));
    }

#ifdef ATH6KL_DISABLE_TARGET_DBGLOGS
    {
        u32 param;

        if (BMIReadMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4)!= 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIReadMemory for disabling debug logs failed\n"));
            return A_ERROR;
        }

        param |= HI_OPTION_DISABLE_DBGLOG;

        if (BMIWriteMemory(ar->arHifDevice,
            HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
            (u8 *)&param,
            4) != 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMIWriteMemory for HI_OPTION_DISABLE_DBGLOG\n"));
            return A_ERROR;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("Firmware mode set\n"));
    }
#endif  

     

	if (ar->arTargetType == TARGET_TYPE_AR6003) {
		u32 ramReservedSz;
		if (ar->arVersion.target_ver == AR6003_REV2_VERSION) {
			param = AR6003_REV2_BOARD_EXT_DATA_ADDRESS;
			ramReservedSz =  AR6003_REV2_RAM_RESERVE_SIZE;
                } else {
			param = AR6003_REV3_BOARD_EXT_DATA_ADDRESS;
			ramReservedSz =  AR6003_REV3_RAM_RESERVE_SIZE;
		}
		if (BMIWriteMemory(ar->arHifDevice,
			HOST_INTEREST_ITEM_ADDRESS(ar, hi_board_ext_data),
						   (u8 *)&param, 4) != 0) {
				AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
						("BMIWriteMemory for "
						 "hi_board_ext_data failed\n"));
				return A_ERROR;
		}
		if (BMIWriteMemory(ar->arHifDevice,
				   HOST_INTEREST_ITEM_ADDRESS(ar,
				   hi_end_RAM_reserve_sz),
				   (u8 *)&ramReservedSz, 4) != 0) {
			AR_DEBUG_PRINTF(ATH_DEBUG_ERR ,
					("BMIWriteMemory for "
					 "hi_end_RAM_reserve_sz failed\n"));
			return A_ERROR;
		}
	}

         

    if (ar6000_set_htc_params(ar->arHifDevice, ar->arTargetType,
			      mbox_yield_limit, 0)) {
				 
        return A_ERROR;
    }

    if (setupbtdev != 0) {
        if (ar6000_set_hci_bridge_flags(ar->arHifDevice,
					ar->arTargetType,
					setupbtdev)) {
            return A_ERROR;
        }
    }
    return 0;
}