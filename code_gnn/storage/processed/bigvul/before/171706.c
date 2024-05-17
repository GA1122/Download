bt_status_t btif_dut_mode_send(uint16_t opcode, uint8_t *buf, uint8_t len)
{
  
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
 if (!btif_is_dut_mode()) {
         BTIF_TRACE_ERROR("Bluedroid HAL needs to be init with test_mode set to 1.");
 return BT_STATUS_FAIL;
 }
    BTM_VendorSpecificCommand(opcode, len, buf, btif_dut_mode_cback);
 return BT_STATUS_SUCCESS;
}
