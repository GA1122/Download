static void btif_dm_pin_req_evt(tBTA_DM_PIN_REQ *p_pin_req)
{
 bt_bdaddr_t bd_addr;
 bt_bdname_t bd_name;
    UINT32 cod;
 bt_pin_code_t pin_code;
 int dev_type;

  
 if (!btif_get_device_type(p_pin_req->bd_addr, &dev_type))
 {
        dev_type = BT_DEVICE_TYPE_BREDR;
 }
    btif_update_remote_properties(p_pin_req->bd_addr, p_pin_req->bd_name,
                                  p_pin_req->dev_class, (tBT_DEVICE_TYPE) dev_type);

    bdcpy(bd_addr.address, p_pin_req->bd_addr);
    memcpy(bd_name.name, p_pin_req->bd_name, BD_NAME_LEN);

    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);

    cod = devclass2uint(p_pin_req->dev_class);

 if (cod == 0) {
        BTIF_TRACE_DEBUG("%s cod is 0, set as unclassified", __func__);
        cod = COD_UNCLASSIFIED;
 }

  
 if (pairing_cb.is_local_initiated && (p_pin_req->min_16_digit == FALSE))
 {
 if (check_cod(&bd_addr, COD_AV_HEADSETS) ||
            check_cod(&bd_addr, COD_AV_HANDSFREE) ||
            check_cod(&bd_addr, COD_AV_HEADPHONES) ||
            check_cod(&bd_addr, COD_AV_PORTABLE_AUDIO) ||
            check_cod(&bd_addr, COD_AV_HIFI_AUDIO) ||
            check_cod(&bd_addr, COD_HID_POINTING))
 {
            BTIF_TRACE_DEBUG("%s()cod matches for auto pair", __FUNCTION__);
  
 if ((btif_storage_is_device_autopair_blacklisted(&bd_addr) == FALSE) &&
 (pairing_cb.autopair_attempts == 0))
 {
                BTIF_TRACE_DEBUG("%s() Attempting auto pair", __FUNCTION__);
                pin_code.pin[0] = 0x30;
                pin_code.pin[1] = 0x30;
                pin_code.pin[2] = 0x30;
                pin_code.pin[3] = 0x30;

                pairing_cb.autopair_attempts++;
                BTA_DmPinReply( (UINT8*)bd_addr.address, TRUE, 4, pin_code.pin);
 return;
 }
 }
 else if (check_cod(&bd_addr, COD_HID_KEYBOARD) ||
                 check_cod(&bd_addr, COD_HID_COMBO))
 {
 if(( btif_storage_is_fixed_pin_zeros_keyboard (&bd_addr) == TRUE) &&
 (pairing_cb.autopair_attempts == 0))
 {
                BTIF_TRACE_DEBUG("%s() Attempting auto pair", __FUNCTION__);
                pin_code.pin[0] = 0x30;
                pin_code.pin[1] = 0x30;
                pin_code.pin[2] = 0x30;
                pin_code.pin[3] = 0x30;

                pairing_cb.autopair_attempts++;
                BTA_DmPinReply( (UINT8*)bd_addr.address, TRUE, 4, pin_code.pin);
 return;
 }
 }
 }
    HAL_CBACK(bt_hal_cbacks, pin_request_cb,
 &bd_addr, &bd_name, cod, p_pin_req->min_16_digit);
}
