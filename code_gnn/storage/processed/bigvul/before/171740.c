static void bte_scan_filt_param_cfg_evt(UINT8 action_type,
                                        tBTA_DM_BLE_PF_AVBL_SPACE avbl_space,
                                        tBTA_DM_BLE_REF_VALUE ref_value, tBTA_STATUS status)
{
  
 if(BTA_SUCCESS != status)
 {
        BTIF_TRACE_ERROR("%s, %d", __FUNCTION__, status);
 }
 else
 {
        BTIF_TRACE_DEBUG("%s", __FUNCTION__);
 }
}
