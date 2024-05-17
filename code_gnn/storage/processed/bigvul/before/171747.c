static void btif_dm_ble_rx_test_cback(void *p)
{
    btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_LE_RX_TEST,
 (char *)p, 1, NULL);
}
