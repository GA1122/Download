ar6000_sysfs_bmi_get_config(struct ar6_softc *ar, u32 mode)
{
    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("BMI: Requesting device specific configuration\n"));

    if (mode == WLAN_INIT_MODE_UDEV) {
        char version[16];
        const struct firmware *fw_entry;

         
        sprintf(version, "%2.2x", ar->arVersion.target_ver);
        if ((A_REQUEST_FIRMWARE(&fw_entry, version, ((struct device *)ar->osDevInfo.pOSDevice))) != 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("BMI: Failure to get configuration for target version: %s\n", version));
            return A_ERROR;
        }

        A_RELEASE_FIRMWARE(fw_entry);
    } else {
         
        int status;
        u32 param, options, sleep, address;

         
        address = MBOX_BASE_ADDRESS + LOCAL_SCRATCH_ADDRESS;
        bmifn(BMIReadSOCRegister(ar->arHifDevice, address, &param));
        options = param;
        param |= AR6K_OPTION_SLEEP_DISABLE;
        bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

        address = RTC_BASE_ADDRESS + SYSTEM_SLEEP_ADDRESS;
        bmifn(BMIReadSOCRegister(ar->arHifDevice, address, &param));
        sleep = param;
        param |= WLAN_SYSTEM_SLEEP_DISABLE_SET(1);
        bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));
        AR_DEBUG_PRINTF(ATH_DEBUG_INFO, ("old options: %d, old sleep: %d\n", options, sleep));

        if (ar->arTargetType == TARGET_TYPE_AR6003) {
             
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, ANALOG_INTF_BASE_ADDRESS + 0x284, 0xF9104001));
             
            param = CPU_CLOCK_STANDARD_SET(1);
        } else {
             
            param = CPU_CLOCK_STANDARD_SET(0);
        }
        address = RTC_BASE_ADDRESS + CPU_CLOCK_ADDRESS;
        bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

        param = 0;
        if (ar->arTargetType == TARGET_TYPE_AR6002) {
            bmifn(BMIReadMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_ext_clk_detected), (u8 *)&param, 4));
        }

         
        if (param != 1) {
            address = RTC_BASE_ADDRESS + LPO_CAL_ADDRESS;
            param = LPO_CAL_ENABLE_SET(1);
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));
        }

         
        if (ar->arVersion.target_ver == AR6003_REV2_VERSION) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("AR6K: Temporary WAR to avoid SDIO CRC error\n"));
            param = 0x20;
            address = GPIO_BASE_ADDRESS + GPIO_PIN10_ADDRESS;
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

            address = GPIO_BASE_ADDRESS + GPIO_PIN11_ADDRESS;
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

            address = GPIO_BASE_ADDRESS + GPIO_PIN12_ADDRESS;
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

            address = GPIO_BASE_ADDRESS + GPIO_PIN13_ADDRESS;
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));
        }

#ifdef FORCE_INTERNAL_CLOCK
         
        if (ar->arTargetType == TARGET_TYPE_AR6003) {
             
            param = 0;
            bmifn(BMIWriteMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_ext_clk_detected), (u8 *)&param, 4));

             
            address = RTC_BASE_ADDRESS + CLOCK_CONTROL_ADDRESS;
            bmifn(BMIReadSOCRegister(ar->arHifDevice, address, &param));
            param &= (~CLOCK_CONTROL_LF_CLK32_SET(1));
            bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));
        }
#endif  

         
        if (ar->arTargetType == TARGET_TYPE_AR6003) {
             
            bmifn(BMIReadMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_board_data), (u8 *)&address, 4));
            AR_DEBUG_PRINTF(ATH_DEBUG_INFO, ("Board Data download address: 0x%x\n", address));

             
            if ((ar6000_transfer_bin_file(ar, AR6K_BOARD_DATA_FILE, address, false)) != 0) {
                return A_ERROR;
            }

             
            param = 1;
            bmifn(BMIWriteMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_board_data_initialized), (u8 *)&param, 4));

             
	    AR6K_APP_LOAD_ADDRESS(address, ar->arVersion.target_ver);
	    if (ar->arVersion.target_ver == AR6003_REV3_VERSION)
		  address = 0x1234;
            status = ar6000_transfer_bin_file(ar, AR6K_OTP_FILE, address, true);
            if (status == 0) {
                 
                param = 0;
                AR6K_APP_START_OVERRIDE_ADDRESS(address, ar->arVersion.target_ver);
                bmifn(BMIExecute(ar->arHifDevice, address, &param));
            } else if (status != A_ENOENT) {
                return A_ERROR;
            } 
        } else {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Programming of board data for chip %d not supported\n", ar->arTargetType));
            return A_ERROR;
        }

         
        AR6K_APP_LOAD_ADDRESS(address, ar->arVersion.target_ver);
        if (ar->arVersion.target_ver == AR6003_REV3_VERSION)
                address = 0x1234;
        if ((ar6000_transfer_bin_file(ar, AR6K_FIRMWARE_FILE, address, true)) != 0) {
            return A_ERROR;
        }

         
        AR6K_APP_START_OVERRIDE_ADDRESS(address, ar->arVersion.target_ver);
        bmifn(BMISetAppStart(ar->arHifDevice, address));

	if(ar->arTargetType == TARGET_TYPE_AR6003) {
		AR6K_DATASET_PATCH_ADDRESS(address, ar->arVersion.target_ver);
		if ((ar6000_transfer_bin_file(ar, AR6K_PATCH_FILE,
					      address, false)) != 0)
			return A_ERROR;
		param = address;
		bmifn(BMIWriteMemory(ar->arHifDevice,
		HOST_INTEREST_ITEM_ADDRESS(ar, hi_dset_list_head),
					   (unsigned char *)&param, 4));
	}

         
        address = RTC_BASE_ADDRESS + SYSTEM_SLEEP_ADDRESS;
        bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, sleep));

        address = MBOX_BASE_ADDRESS + LOCAL_SCRATCH_ADDRESS;
        param = options | 0x20;
        bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));

        if (ar->arTargetType == TARGET_TYPE_AR6003) {
             
#ifndef CONFIG_AR600x_DEBUG_UART_TX_PIN
#define CONFIG_AR600x_DEBUG_UART_TX_PIN 8
#endif
            param = CONFIG_AR600x_DEBUG_UART_TX_PIN;
            bmifn(BMIWriteMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_dbg_uart_txpin), (u8 *)&param, 4));

#if (CONFIG_AR600x_DEBUG_UART_TX_PIN == 23)
            {
                address = GPIO_BASE_ADDRESS + CLOCK_GPIO_ADDRESS;
                bmifn(BMIReadSOCRegister(ar->arHifDevice, address, &param));
                param |= CLOCK_GPIO_BT_CLK_OUT_EN_SET(1);
                bmifn(BMIWriteSOCRegister(ar->arHifDevice, address, param));
            }
#endif

             
#ifdef ATH6KL_CONFIG_GPIO_BT_RESET
#define CONFIG_AR600x_BT_RESET_PIN	0x16
            param = CONFIG_AR600x_BT_RESET_PIN;
            bmifn(BMIWriteMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_hci_uart_support_pins), (u8 *)&param, 4));
#endif  

             
#ifndef CONFIG_ATH6KL_BT_UART_FC_POLARITY
#define CONFIG_ATH6KL_BT_UART_FC_POLARITY 0
#endif

#if (CONFIG_ATH6KL_BT_UART_FC_POLARITY == 1)
            if (ar->arVersion.target_ver == AR6003_REV2_VERSION) {
                param = ((CONFIG_ATH6KL_BT_UART_FC_POLARITY << 1) & 0x2);
                bmifn(BMIWriteMemory(ar->arHifDevice, HOST_INTEREST_ITEM_ADDRESS(ar, hi_hci_uart_pwr_mgmt_params), (u8 *)&param, 4));
            }
#endif  
        }

#ifdef HTC_RAW_INTERFACE
        if (!eppingtest && bypasswmi) {
             
            resetok = 0;
            msleep(1000);
        }
#endif  
    }

    return 0;
}