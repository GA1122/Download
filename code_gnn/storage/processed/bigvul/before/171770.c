void btif_dm_on_disable()
{
  
 if (pairing_cb.state == BT_BOND_STATE_BONDING)
 {
 bt_bdaddr_t bd_addr;

        BTIF_TRACE_DEBUG("%s: Cancel pending pairing request", __FUNCTION__);
        bdcpy(bd_addr.address, pairing_cb.bd_addr);
        btif_dm_cancel_bond(&bd_addr);
 }
}
