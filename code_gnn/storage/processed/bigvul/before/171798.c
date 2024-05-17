BOOLEAN check_sdp_bl(const bt_bdaddr_t *remote_bdaddr)
{
    UINT16 manufacturer = 0;
    UINT8 lmp_ver = 0;
    UINT16 lmp_subver = 0;
 bt_property_t prop_name;
 bt_remote_version_t info;


 if (remote_bdaddr == NULL)
 return FALSE;

 
    BTM_ReadRemoteVersion(*(BD_ADDR*)remote_bdaddr, &lmp_ver,
 &manufacturer, &lmp_subver);



  
    BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_REMOTE_VERSION_INFO,
 sizeof(bt_remote_version_t), &info);

 if (btif_storage_get_remote_device_property((bt_bdaddr_t *)remote_bdaddr,
 &prop_name) != BT_STATUS_SUCCESS)
 {

 return FALSE;
 }
    manufacturer = info.manufacturer;

 for (int i = 0; i < MAX_SDP_BL_ENTRIES; i++)
 {
 if (manufacturer == sdp_blacklist[i].manufact_id)
 return TRUE;
 }
 return FALSE;
}
