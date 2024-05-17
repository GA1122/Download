static void bond_state_changed(bt_status_t status, bt_bdaddr_t *bd_addr, bt_bond_state_t state)
{
 if ((pairing_cb.state == state) && (state == BT_BOND_STATE_BONDING))
 {
 if (!bdaddr_is_empty(&pairing_cb.static_bdaddr))
 {
            HAL_CBACK(bt_hal_cbacks, bond_state_changed_cb, status, bd_addr, state);
 }
 return;
 }

 if (pairing_cb.bond_type == BOND_TYPE_TEMPORARY)
        state = BT_BOND_STATE_NONE;

    BTIF_TRACE_DEBUG("%s: state=%d, prev_state=%d, sdp_attempts = %d", __func__,
                      state, pairing_cb.state, pairing_cb.sdp_attempts);

    HAL_CBACK(bt_hal_cbacks, bond_state_changed_cb, status, bd_addr, state);

 if (state == BT_BOND_STATE_BONDING)
 {
        pairing_cb.state = state;
        bdcpy(pairing_cb.bd_addr, bd_addr->address);
 } else {
 if (!pairing_cb.sdp_attempts)
            memset(&pairing_cb, 0, sizeof(pairing_cb));
 else
            BTIF_TRACE_DEBUG("%s: BR-EDR service discovery active", __func__);
 }
}
