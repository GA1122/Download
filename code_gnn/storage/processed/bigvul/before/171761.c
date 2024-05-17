bt_status_t btif_dm_get_adapter_property(bt_property_t *prop)
{
    BTIF_TRACE_EVENT("%s: type=0x%x", __FUNCTION__, prop->type);
 switch (prop->type)
 {
 case BT_PROPERTY_BDNAME:
 {
 bt_bdname_t *bd_name = (bt_bdname_t*)prop->val;
            strncpy((char *)bd_name->name, (char *)btif_get_default_local_name(),
 sizeof(bd_name->name) - 1);
            bd_name->name[sizeof(bd_name->name) - 1] = 0;
            prop->len = strlen((char *)bd_name->name);
 }
 break;

 case BT_PROPERTY_ADAPTER_SCAN_MODE:
 {
  
 bt_scan_mode_t *mode = (bt_scan_mode_t*)prop->val;
 *mode = BT_SCAN_MODE_NONE;
            prop->len = sizeof(bt_scan_mode_t);
 }
 break;

 case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
 {
 uint32_t *tmt = (uint32_t*)prop->val;
 *tmt = 120;  
            prop->len = sizeof(uint32_t);
 }
 break;

 default:
            prop->len = 0;
 return BT_STATUS_FAIL;
 }
 return BT_STATUS_SUCCESS;
}
