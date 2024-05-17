void btm_sec_clear_ble_keys (tBTM_SEC_DEV_REC  *p_dev_rec)
{

    BTM_TRACE_DEBUG ("btm_sec_clear_ble_keys: Clearing BLE Keys");
#if (SMP_INCLUDED== TRUE)
    p_dev_rec->ble.key_type = 0;
    memset (&p_dev_rec->ble.keys, 0, sizeof(tBTM_SEC_BLE_KEYS));
#endif
    gatt_delete_dev_from_srv_chg_clt_list(p_dev_rec->bd_addr);
}
