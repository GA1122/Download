bt_status_t btif_set_adapter_property(const bt_property_t *property)
{
 btif_storage_req_t req;
 bt_status_t status = BT_STATUS_SUCCESS;
 int storage_req_id = BTIF_CORE_STORAGE_NOTIFY_STATUS;  
 char bd_name[BTM_MAX_LOC_BD_NAME_LEN +1];
    UINT16  name_len = 0;

    BTIF_TRACE_EVENT("btif_set_adapter_property type: %d, len %d, 0x%x",
 property->type, property->len, property->val);

 if (!btif_is_enabled())
 return BT_STATUS_NOT_READY;

 switch(property->type)
 {
 case BT_PROPERTY_BDNAME:
 {
                name_len = property->len > BTM_MAX_LOC_BD_NAME_LEN ? BTM_MAX_LOC_BD_NAME_LEN:
 property->len;
                memcpy(bd_name,property->val, name_len);
                bd_name[name_len] = '\0';

                BTIF_TRACE_EVENT("set property name : %s", (char *)bd_name);

                BTA_DmSetDeviceName((char *)bd_name);

                storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
 }
 break;

 case BT_PROPERTY_ADAPTER_SCAN_MODE:
 {
 bt_scan_mode_t mode = *(bt_scan_mode_t*)property->val;
                tBTA_DM_DISC disc_mode;
                tBTA_DM_CONN conn_mode;

 switch(mode)
 {
 case BT_SCAN_MODE_NONE:
                        disc_mode = BTA_DM_NON_DISC;
                        conn_mode = BTA_DM_NON_CONN;
 break;

 case BT_SCAN_MODE_CONNECTABLE:
                        disc_mode = BTA_DM_NON_DISC;
                        conn_mode = BTA_DM_CONN;
 break;

 case BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE:
                        disc_mode = BTA_DM_GENERAL_DISC;
                        conn_mode = BTA_DM_CONN;
 break;

 default:
                        BTIF_TRACE_ERROR("invalid scan mode (0x%x)", mode);
 return BT_STATUS_PARM_INVALID;
 }

                BTIF_TRACE_EVENT("set property scan mode : %x", mode);

                BTA_DmSetVisibility(disc_mode, conn_mode, BTA_DM_IGNORE, BTA_DM_IGNORE);

                storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
 }
 break;
 case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
 {
  
                storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
 }
 break;
 case BT_PROPERTY_BDADDR:
 case BT_PROPERTY_UUIDS:
 case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
 case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
  
            status = BT_STATUS_FAIL;
 break;
 default:
            BTIF_TRACE_ERROR("btif_get_adapter_property : invalid type %d",
 property->type);
            status = BT_STATUS_FAIL;
 break;
 }

 if (storage_req_id != BTIF_CORE_STORAGE_NO_ACTION)
 {
  

        memset(&(req.write_req.bd_addr), 0, sizeof(bt_bdaddr_t));
        memcpy(&(req.write_req.prop), property, sizeof(bt_property_t));

 return btif_transfer_context(execute_storage_request,
                                     storage_req_id,
 (char*)&req,
 sizeof(btif_storage_req_t)+property->len,
                                     btif_in_storage_request_copy_cb);
 }

 return status;

}
