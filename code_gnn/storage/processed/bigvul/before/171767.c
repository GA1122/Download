void btif_dm_hh_open_failed(bt_bdaddr_t *bdaddr)
{
 if (pairing_cb.state == BT_BOND_STATE_BONDING &&
            bdcmp(bdaddr->address, pairing_cb.bd_addr) == 0)
 {
        bond_state_changed(BT_STATUS_FAIL, bdaddr, BT_BOND_STATE_NONE);
 }
}
