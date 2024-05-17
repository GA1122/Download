static int ssp_reply(const bt_bdaddr_t *bd_addr, bt_ssp_variant_t variant,
 uint8_t accept, uint32_t passkey)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_ssp_reply(bd_addr, variant, accept, passkey);
}
