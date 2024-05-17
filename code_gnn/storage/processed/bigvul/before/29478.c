int wvlan_uil_get_info(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	int i;
	 

	DBG_FUNC("wvlan_uil_get_info");
	DBG_ENTER(DbgInfo);

	if (urq->hcfCtx == &(lp->hcfCtx)) {
		if ((urq->data != NULL) && (urq->len != 0)) {
			ltv_t      *pLtv;
			bool_t      ltvAllocated = FALSE;

			 
			if (urq->len < (sizeof(hcf_16) * 2)) {
				urq->len = sizeof(lp->ltvRecord);
				DBG_ERROR(DbgInfo, "No Length/Type in LTV!!!\n");
				DBG_ERROR(DbgInfo, "UIL_ERR_LEN\n");
				urq->result = UIL_ERR_LEN;
				DBG_LEAVE(DbgInfo);
				return result;
			}

			 
			result = verify_area(VERIFY_READ, urq->data, sizeof(hcf_16) * 2);
			if (result != 0) {
				DBG_ERROR(DbgInfo, "verify_area(), VERIFY_READ FAILED\n");
				urq->result = UIL_FAILURE;
				DBG_LEAVE(DbgInfo);
				return result;
			}

			 
			result = copy_from_user(&(lp->ltvRecord), urq->data, sizeof(hcf_16) * 2);

			 
			if (((lp->ltvRecord.len + 1) * sizeof(hcf_16)) > urq->len) {
				DBG_ERROR(DbgInfo, "Incoming LTV too big\n");
				urq->len = sizeof(lp->ltvRecord);
				urq->result = UIL_ERR_LEN;
				DBG_LEAVE(DbgInfo);
				return result;
			}

			 
			switch (lp->ltvRecord.typ) {
			case CFG_NIC_IDENTITY:
				memcpy(&lp->ltvRecord.u.u8[0], &lp->NICIdentity, sizeof(lp->NICIdentity));
				break;
			case CFG_PRI_IDENTITY:
				memcpy(&lp->ltvRecord.u.u8[0], &lp->PrimaryIdentity, sizeof(lp->PrimaryIdentity));
				break;
			case CFG_AP_MODE:
				DBG_ERROR(DbgInfo, "set CFG_AP_MODE no longer supported, so is get useful ????\n");
				lp->ltvRecord.u.u16[0] =
					CNV_INT_TO_LITTLE(lp->hcfCtx.IFB_FWIdentity.comp_id) == COMP_ID_FW_AP;
				break;
			 
			case CFG_ENCRYPT_STRING:
			case CFG_COUNTRY_STRING:
			case CFG_DRIVER_ENABLE:
			case CFG_WOLAS_ENABLE:
				 
				urq->result = UIL_FAILURE;
				break;
			case CFG_DRV_INFO:
				DBG_TRACE(DbgInfo, "Intercept CFG_DRV_INFO\n");
				result = cfg_driver_info(urq, lp);
				break;
			case CFG_DRV_IDENTITY:
				DBG_TRACE(DbgInfo, "Intercept CFG_DRV_IDENTITY\n");
				result = cfg_driver_identity(urq, lp);
				break;
			case CFG_IFB:
				 
				if (!capable(CAP_NET_ADMIN)) {
					result = -EPERM;
					break;
				}

				 

			case CFG_FW_IDENTITY:    
			default:

				 
				result = verify_area(VERIFY_WRITE, urq->data, urq->len);
				if (result != 0) {
					DBG_ERROR(DbgInfo, "verify_area(), VERIFY_WRITE FAILED\n");
					urq->result = UIL_FAILURE;
					break;
				}

				 
				if (urq->len > sizeof(lp->ltvRecord)) {
					pLtv = kmalloc(urq->len, GFP_KERNEL);
					if (pLtv != NULL) {
						ltvAllocated = TRUE;

						 
						memcpy(pLtv, &(lp->ltvRecord), sizeof(hcf_16) * 2);
					} else {
						urq->len = sizeof(lp->ltvRecord);
						urq->result = UIL_ERR_LEN;
						DBG_ERROR(DbgInfo, "kmalloc FAILED\n");
						DBG_ERROR(DbgInfo, "UIL_ERR_LEN\n");
						result = -ENOMEM;
						break;
					}
				} else {
					pLtv = &(lp->ltvRecord);
				}

				wl_act_int_off(lp);
				urq->result = hcf_get_info(&(lp->hcfCtx), (LTVP) pLtv);
				wl_act_int_on(lp);

				 
				 

				 

				 
				break;
			}

			 
			switch (lp->ltvRecord.typ) {
			 
			case CFG_CNF_PORT_TYPE:
			case CFG_CNF_OWN_CHANNEL:
			case CFG_CNF_OWN_ATIM_WINDOW:
			case CFG_CNF_SYSTEM_SCALE:
			case CFG_CNF_MAX_DATA_LEN:
			case CFG_CNF_PM_ENABLED:
			case CFG_CNF_MCAST_RX:
			case CFG_CNF_MAX_SLEEP_DURATION:
			case CFG_CNF_HOLDOVER_DURATION:
			case CFG_CNF_OWN_DTIM_PERIOD:
			case CFG_CNF_MCAST_PM_BUF:
			case CFG_CNF_REJECT_ANY:
			case CFG_CNF_ENCRYPTION:
			case CFG_CNF_AUTHENTICATION:
			case CFG_CNF_EXCL_UNENCRYPTED:
			case CFG_CNF_INTRA_BSS_RELAY:
			case CFG_CNF_MICRO_WAVE:
			case CFG_CNF_LOAD_BALANCING:
			case CFG_CNF_MEDIUM_DISTRIBUTION:
#ifdef WARP
			case CFG_CNF_TX_POW_LVL:
			case CFG_CNF_CONNECTION_CNTL:
			case CFG_CNF_OWN_BEACON_INTERVAL:                           
			case CFG_COEXISTENSE_BEHAVIOUR:                             
			 
#endif  
			case CFG_CREATE_IBSS:
			case CFG_RTS_THRH:
			case CFG_PROMISCUOUS_MODE:
			 
			case CFG_RTS_THRH0:
			case CFG_RTS_THRH1:
			case CFG_RTS_THRH2:
			case CFG_RTS_THRH3:
			case CFG_RTS_THRH4:
			case CFG_RTS_THRH5:
			case CFG_RTS_THRH6:
			case CFG_TX_RATE_CNTL0:
			case CFG_TX_RATE_CNTL1:
			case CFG_TX_RATE_CNTL2:
			case CFG_TX_RATE_CNTL3:
			case CFG_TX_RATE_CNTL4:
			case CFG_TX_RATE_CNTL5:
			case CFG_TX_RATE_CNTL6:
			case CFG_TX_KEY_ID:
			case CFG_TICK_TIME:
			case CFG_MAX_LOAD_TIME:
			case CFG_NIC_TEMP_TYPE:
			case CFG_PORT_STAT:
			case CFG_CUR_TX_RATE:
			case CFG_CUR_BEACON_INTERVAL:
			case CFG_PROTOCOL_RSP_TIME:
			case CFG_CUR_SHORT_RETRY_LIMIT:
			case CFG_CUR_LONG_RETRY_LIMIT:
			case CFG_MAX_TX_LIFETIME:
			case CFG_MAX_RX_LIFETIME:
			case CFG_CF_POLLABLE:
			case CFG_PRIVACY_OPT_IMPLEMENTED:
			 
			 
			 
			 
			 
			 
			 
			 
			 
			case CFG_PHY_TYPE:
			case CFG_CUR_CHANNEL:
			 
			 
			 
			 
			 
			case CFG_CNF_OWN_SSID:
			case CFG_CNF_OWN_NAME:
			 
			case CFG_DESIRED_SSID:
			case CFG_SCAN_SSID:
			case CFG_CUR_SSID:
				lp->ltvRecord.u.u16[0] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[0]);
				break;
			 
			case CFG_CNF_OWN_MAC_ADDR:
			 
			case CFG_CNF_WDS_ADDR1:
			case CFG_CNF_WDS_ADDR2:
			case CFG_CNF_WDS_ADDR3:
			case CFG_CNF_WDS_ADDR4:
			case CFG_CNF_WDS_ADDR5:
			case CFG_CNF_WDS_ADDR6:
			case CFG_GROUP_ADDR:
			case CFG_NIC_SERIAL_NUMBER:
			case CFG_CUR_BSSID:
			case CFG_NIC_MAC_ADDR:
			case CFG_SUPPORTED_DATA_RATES:   
				break;
			       
			   
			 

			case CFG_DEFAULT_KEYS:
				{
					CFG_DEFAULT_KEYS_STRCT *pKeys = (CFG_DEFAULT_KEYS_STRCT *)&lp->ltvRecord.u.u8[0];

					pKeys[0].len = CNV_INT_TO_LITTLE(pKeys[0].len);
					pKeys[1].len = CNV_INT_TO_LITTLE(pKeys[1].len);
					pKeys[2].len = CNV_INT_TO_LITTLE(pKeys[2].len);
					pKeys[3].len = CNV_INT_TO_LITTLE(pKeys[3].len);
				}
				break;
			case CFG_CNF_MCAST_RATE:
			case CFG_TX_RATE_CNTL:
			case CFG_SUPPORTED_RATE_SET_CNTL:     
			case CFG_BASIC_RATE_SET_CNTL:     
				lp->ltvRecord.u.u16[0] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[0]);
				lp->ltvRecord.u.u16[1] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[1]);
				break;
			case CFG_DL_BUF:
			case CFG_NIC_IDENTITY:
			case CFG_COMMS_QUALITY:
			case CFG_PCF_INFO:
				lp->ltvRecord.u.u16[0] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[0]);
				lp->ltvRecord.u.u16[1] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[1]);
				lp->ltvRecord.u.u16[2] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[2]);
				break;
			case CFG_FW_IDENTITY:
				lp->ltvRecord.u.u16[0] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[0]);
				lp->ltvRecord.u.u16[1] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[1]);
				lp->ltvRecord.u.u16[2] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[2]);
				lp->ltvRecord.u.u16[3] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[3]);
				break;
			 
			case CFG_NIC_MFI_SUP_RANGE:
			case CFG_NIC_CFI_SUP_RANGE:
			case CFG_NIC_PROFILE:
			case CFG_FW_SUP_RANGE:
				lp->ltvRecord.u.u16[0] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[0]);
				lp->ltvRecord.u.u16[1] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[1]);
				lp->ltvRecord.u.u16[2] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[2]);
				lp->ltvRecord.u.u16[3] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[3]);
				lp->ltvRecord.u.u16[4] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[4]);
				break;
			case CFG_MFI_ACT_RANGES_STA:
			case CFG_CFI_ACT_RANGES_STA:
			case CFG_CUR_SCALE_THRH:
			case CFG_AUTHENTICATION_ALGORITHMS:
				for (i = 0; i < (lp->ltvRecord.len - 1); i++)
					lp->ltvRecord.u.u16[i] = CNV_INT_TO_LITTLE(lp->ltvRecord.u.u16[i]);
				break;
			 
			case CFG_PRI_IDENTITY:
				break;
			case CFG_MB_INFO:
				 
				break;
			 
			case CFG_IFB:
			case CFG_DRV_INFO:
			case CFG_AP_MODE:
			case CFG_ENCRYPT_STRING:
			case CFG_COUNTRY_STRING:
			case CFG_DRIVER_ENABLE:
			case CFG_WOLAS_ENABLE:
			default:
				break;
			}

			 
			copy_to_user(urq->data, &(lp->ltvRecord), urq->len);

			if (ltvAllocated)
				kfree(&(lp->ltvRecord));
			urq->result = UIL_SUCCESS;
		} else {
			urq->result = UIL_FAILURE;
		}
	} else {
		DBG_ERROR(DbgInfo, "UIL_ERR_WRONG_IFB\n");
		urq->result = UIL_ERR_WRONG_IFB;
	}

	DBG_LEAVE(DbgInfo);
	return result;
}  
