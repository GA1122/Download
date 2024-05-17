int dut_mode_send(uint16_t opcode, uint8_t* buf, uint8_t len)
{
    LOG_INFO("dut_mode_send");

  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dut_mode_send(opcode, buf, len);
}
