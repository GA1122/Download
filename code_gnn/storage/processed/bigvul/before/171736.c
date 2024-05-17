static void bta_energy_info_cb(tBTA_DM_BLE_TX_TIME_MS tx_time, tBTA_DM_BLE_RX_TIME_MS rx_time,
                                    tBTA_DM_BLE_IDLE_TIME_MS idle_time,
                                    tBTA_DM_BLE_ENERGY_USED energy_used,
                                    tBTA_DM_CONTRL_STATE ctrl_state, tBTA_STATUS status)
{
    BTIF_TRACE_DEBUG("energy_info_cb-Status:%d,state=%d,tx_t=%ld, rx_t=%ld, idle_time=%ld,used=%ld",
        status, ctrl_state, tx_time, rx_time, idle_time, energy_used);

 btif_activity_energy_info_cb_t btif_cb;
    btif_cb.status = status;
    btif_cb.ctrl_state = ctrl_state;
    btif_cb.tx_time = (uint64_t) tx_time;
    btif_cb.rx_time = (uint64_t) rx_time;
    btif_cb.idle_time =(uint64_t) idle_time;
    btif_cb.energy_used =(uint64_t) energy_used;
    btif_transfer_context(btif_dm_upstreams_evt, BTA_DM_ENER_INFO_READ,
 (char*) &btif_cb, sizeof(btif_activity_energy_info_cb_t), NULL);
}
