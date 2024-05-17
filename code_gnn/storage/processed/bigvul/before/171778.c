void btif_dm_remove_ble_bonding_keys(void)
{
 bt_bdaddr_t bd_addr;

    BTIF_TRACE_DEBUG("%s",__FUNCTION__ );

    bdcpy(bd_addr.address, pairing_cb.bd_addr);
    btif_storage_remove_ble_bonding_keys(&bd_addr);
}
