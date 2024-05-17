static BOOLEAN check_cached_remote_name(tBTA_DM_SEARCH *p_search_data,
                                UINT8 *p_remote_name, UINT8 *p_remote_name_len)
{
 bt_bdname_t bdname;
 bt_bdaddr_t remote_bdaddr;
 bt_property_t prop_name;

  
    bdcpy(remote_bdaddr.address, p_search_data->inq_res.bd_addr);
    BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_BDNAME,
 sizeof(bt_bdname_t), &bdname);
 if (btif_storage_get_remote_device_property(
 &remote_bdaddr, &prop_name) == BT_STATUS_SUCCESS)
 {
 if (p_remote_name && p_remote_name_len)
 {
            strcpy((char *)p_remote_name, (char *)bdname.name);
 *p_remote_name_len = strlen((char *)p_remote_name);
 }
 return TRUE;
 }

 return FALSE;
}
