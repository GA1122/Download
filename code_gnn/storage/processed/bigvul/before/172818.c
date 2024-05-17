static int pin_reply(const bt_bdaddr_t *bd_addr, uint8_t accept,
 uint8_t pin_len, bt_pin_code_t *pin_code)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_pin_reply(bd_addr, accept, pin_len, pin_code);
}
