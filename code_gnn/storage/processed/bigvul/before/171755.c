void btif_dm_cb_remove_bond(bt_bdaddr_t *bd_addr)
{
  
  
#if (defined(BTA_HH_INCLUDED) && (BTA_HH_INCLUDED == TRUE))
 if (btif_hh_virtual_unplug(bd_addr) != BT_STATUS_SUCCESS)
#endif
 {
         BTIF_TRACE_DEBUG("%s: Removing HH device", __func__);
         BTA_DmRemoveDevice((UINT8 *)bd_addr->address);
 }
}
