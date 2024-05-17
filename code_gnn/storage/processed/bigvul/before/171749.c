static void btif_dm_ble_test_end_cback(void *p)
{
    btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_LE_TEST_END,
 (char *)p, 3, NULL);
}
