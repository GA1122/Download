static void btif_dm_ssp_cfm_req_evt(tBTA_DM_SP_CFM_REQ *p_ssp_cfm_req)
{
 bt_bdaddr_t bd_addr;
 bt_bdname_t bd_name;
    UINT32 cod;
    BOOLEAN is_incoming = !(pairing_cb.state == BT_BOND_STATE_BONDING);
 int dev_type;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

  
 if (!btif_get_device_type(p_ssp_cfm_req->bd_addr, &dev_type))
 {
        dev_type = BT_DEVICE_TYPE_BREDR;
 }
    btif_update_remote_properties(p_ssp_cfm_req->bd_addr, p_ssp_cfm_req->bd_name,
                                  p_ssp_cfm_req->dev_class, (tBT_DEVICE_TYPE) dev_type);

    bdcpy(bd_addr.address, p_ssp_cfm_req->bd_addr);
    memcpy(bd_name.name, p_ssp_cfm_req->bd_name, BD_NAME_LEN);

  
    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);

  
 if (p_ssp_cfm_req->just_works && !(p_ssp_cfm_req->loc_auth_req & BTM_AUTH_BONDS) &&
 !(p_ssp_cfm_req->rmt_auth_req & BTM_AUTH_BONDS) &&
 !(check_cod((bt_bdaddr_t*)&p_ssp_cfm_req->bd_addr, COD_HID_POINTING)))
        pairing_cb.bond_type = BOND_TYPE_TEMPORARY;
 else
        pairing_cb.bond_type = BOND_TYPE_PERSISTENT;

    btm_set_bond_type_dev(p_ssp_cfm_req->bd_addr, pairing_cb.bond_type);

    pairing_cb.is_ssp = TRUE;

  
 if (p_ssp_cfm_req->just_works)
 {
  
 if (is_incoming && pairing_cb.bond_type != BOND_TYPE_TEMPORARY &&
 ((p_ssp_cfm_req->loc_io_caps == HCI_IO_CAP_DISPLAY_YESNO) &&
 (p_ssp_cfm_req->rmt_io_caps == HCI_IO_CAP_DISPLAY_ONLY ||
                 p_ssp_cfm_req->rmt_io_caps == HCI_IO_CAP_NO_IO)))
 {
            BTIF_TRACE_EVENT("%s: User consent needed for incoming pairing request. loc_io_caps: %d, rmt_io_caps: %d",
                __FUNCTION__, p_ssp_cfm_req->loc_io_caps, p_ssp_cfm_req->rmt_io_caps);
 }
 else
 {
            BTIF_TRACE_EVENT("%s: Auto-accept JustWorks pairing", __FUNCTION__);
            btif_dm_ssp_reply(&bd_addr, BT_SSP_VARIANT_CONSENT, TRUE, 0);
 return;
 }
 }

    cod = devclass2uint(p_ssp_cfm_req->dev_class);

 if (cod == 0) {
        LOG_DEBUG("%s cod is 0, set as unclassified", __func__);
        cod = COD_UNCLASSIFIED;
 }

    pairing_cb.sdp_attempts = 0;
    HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
 (p_ssp_cfm_req->just_works ? BT_SSP_VARIANT_CONSENT : BT_SSP_VARIANT_PASSKEY_CONFIRMATION),
                     p_ssp_cfm_req->num_val);
}
