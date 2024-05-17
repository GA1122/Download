int dut_mode_configure(uint8_t enable)
{
    LOG_INFO("dut_mode_configure");

  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dut_mode_configure(enable);
}
