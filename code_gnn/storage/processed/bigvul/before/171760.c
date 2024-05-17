static void btif_dm_generic_evt(UINT16 event, char* p_param)
{
    BTIF_TRACE_EVENT("%s: event=%d", __FUNCTION__, event);
 switch(event)
 {
 case BTIF_DM_CB_DISCOVERY_STARTED:
 {
            HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb, BT_DISCOVERY_STARTED);
 }
 break;

 case BTIF_DM_CB_CREATE_BOND:
 {
            pairing_cb.timeout_retries = NUM_TIMEOUT_RETRIES;
 btif_dm_create_bond_cb_t *create_bond_cb = (btif_dm_create_bond_cb_t*)p_param;
            btif_dm_cb_create_bond(&create_bond_cb->bdaddr, create_bond_cb->transport);
 }
 break;

 case BTIF_DM_CB_REMOVE_BOND:
 {
            btif_dm_cb_remove_bond((bt_bdaddr_t *)p_param);
 }
 break;

 case BTIF_DM_CB_HID_REMOTE_NAME:
 {
            btif_dm_cb_hid_remote_name((tBTM_REMOTE_DEV_NAME *)p_param);
 }
 break;

 case BTIF_DM_CB_BOND_STATE_BONDING:
 {
                bond_state_changed(BT_STATUS_SUCCESS, (bt_bdaddr_t *)p_param, BT_BOND_STATE_BONDING);
 }
 break;
 case BTIF_DM_CB_LE_TX_TEST:
 case BTIF_DM_CB_LE_RX_TEST:
 {
 uint8_t status;
                STREAM_TO_UINT8(status, p_param);
                HAL_CBACK(bt_hal_cbacks, le_test_mode_cb,
 (status == 0) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, 0);
 }
 break;
 case BTIF_DM_CB_LE_TEST_END:
 {
 uint8_t status;
 uint16_t count = 0;
                STREAM_TO_UINT8(status, p_param);
 if (status == 0)
                    STREAM_TO_UINT16(count, p_param);
                HAL_CBACK(bt_hal_cbacks, le_test_mode_cb,
 (status == 0) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, count);
 }
 break;
 default:
 {
            BTIF_TRACE_WARNING("%s : Unknown event 0x%x", __FUNCTION__, event);
 }
 break;
 }
}
