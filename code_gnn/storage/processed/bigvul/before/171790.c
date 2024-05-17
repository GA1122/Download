bt_status_t btif_le_test_mode(uint16_t opcode, uint8_t *buf, uint8_t len)
{
 switch (opcode) {
 case HCI_BLE_TRANSMITTER_TEST:
 if (len != 3) return BT_STATUS_PARM_INVALID;
             BTM_BleTransmitterTest(buf[0],buf[1],buf[2], btif_dm_ble_tx_test_cback);
 break;
 case HCI_BLE_RECEIVER_TEST:
 if (len != 1) return BT_STATUS_PARM_INVALID;
             BTM_BleReceiverTest(buf[0], btif_dm_ble_rx_test_cback);
 break;
 case HCI_BLE_TEST_END:
             BTM_BleTestEnd((tBTM_CMPL_CB*) btif_dm_ble_test_end_cback);
 break;
 default:
             BTIF_TRACE_ERROR("%s: Unknown LE Test Mode Command 0x%x", __FUNCTION__, opcode);
 return BT_STATUS_UNSUPPORTED;
 }
 return BT_STATUS_SUCCESS;
}
