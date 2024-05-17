int le_test_mode(uint16_t opcode, uint8_t* buf, uint8_t len)
{
    LOG_INFO("le_test_mode");

  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_le_test_mode(opcode, buf, len);
}
