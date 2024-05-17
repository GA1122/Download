bt_status_t btif_dm_create_bond(const bt_bdaddr_t *bd_addr, int transport)
{
 btif_dm_create_bond_cb_t create_bond_cb;
    create_bond_cb.transport = transport;
    bdcpy(create_bond_cb.bdaddr.address, bd_addr->address);

 bdstr_t bdstr;
    BTIF_TRACE_EVENT("%s: bd_addr=%s, transport=%d", __FUNCTION__, bdaddr_to_string(bd_addr, bdstr, sizeof(bdstr)), transport);
 if (pairing_cb.state != BT_BOND_STATE_NONE)
 return BT_STATUS_BUSY;

    btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_CREATE_BOND,
 (char *)&create_bond_cb, sizeof(btif_dm_create_bond_cb_t), NULL);

 return BT_STATUS_SUCCESS;
}
