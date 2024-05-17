static void btif_hh_upstreams_evt(UINT16 event, char* p_param)
{
    tBTA_HH *p_data = (tBTA_HH *)p_param;
 btif_hh_device_t *p_dev = NULL;
 int i;
 int len, tmplen;

    BTIF_TRACE_DEBUG("%s: event=%s", __FUNCTION__, dump_hh_event(event));

 switch (event)
 {
 case BTA_HH_ENABLE_EVT:
            BTIF_TRACE_DEBUG("%s: BTA_HH_ENABLE_EVT: status =%d",__FUNCTION__, p_data->status);
 if (p_data->status == BTA_HH_OK) {
                btif_hh_cb.status = BTIF_HH_ENABLED;
                BTIF_TRACE_DEBUG("%s--Loading added devices",__FUNCTION__);
  
                btif_storage_load_bonded_hid_info();
 }
 else {
                btif_hh_cb.status = BTIF_HH_DISABLED;
                BTIF_TRACE_WARNING("BTA_HH_ENABLE_EVT: Error, HH enabling failed, status = %d", p_data->status);
 }
 break;

 case BTA_HH_DISABLE_EVT:
            btif_hh_cb.status = BTIF_HH_DISABLED;
 if (p_data->status == BTA_HH_OK) {
 int i;
                memset(&btif_hh_cb, 0, sizeof(btif_hh_cb));
 for (i = 0; i < BTIF_HH_MAX_HID; i++){
                    btif_hh_cb.devices[i].dev_status = BTHH_CONN_STATE_UNKNOWN;
 }
 }
 else
                BTIF_TRACE_WARNING("BTA_HH_DISABLE_EVT: Error, HH disabling failed, status = %d", p_data->status);
 break;

 case BTA_HH_OPEN_EVT:
            BTIF_TRACE_WARNING("%s: BTA_HH_OPN_EVT: handle=%d, status =%d",__FUNCTION__, p_data->conn.handle, p_data->conn.status);
 if (p_data->conn.status == BTA_HH_OK) {
                p_dev = btif_hh_find_connected_dev_by_handle(p_data->conn.handle);
 if (p_dev == NULL) {
                    BTIF_TRACE_WARNING("BTA_HH_OPEN_EVT: Error, cannot find device with handle %d", p_data->conn.handle);
                    btif_hh_cb.status = BTIF_HH_DEV_DISCONNECTED;
                    BTA_HhClose(p_data->conn.handle);
                    HAL_CBACK(bt_hh_callbacks, connection_state_cb, (bt_bdaddr_t*) &p_data->conn.bda,BTHH_CONN_STATE_DISCONNECTED);
 }
 else if (p_dev->fd < 0) {
                    BTIF_TRACE_WARNING("BTA_HH_OPEN_EVT: Error, failed to find the uhid driver...");
                    memcpy(&(p_dev->bd_addr), p_data->conn.bda, BD_ADDR_LEN);
                    btif_hh_cb.status = BTIF_HH_DEV_DISCONNECTED;
                    BTA_HhClose(p_data->conn.handle);
 }
 else {
                    BTIF_TRACE_WARNING("BTA_HH_OPEN_EVT: Found device...Getting dscp info for handle ... %d",p_data->conn.handle);
                    memcpy(&(p_dev->bd_addr), p_data->conn.bda, BD_ADDR_LEN);
                    btif_hh_cb.status = BTIF_HH_DEV_CONNECTED;
 if (check_cod((bt_bdaddr_t*)p_data->conn.bda, COD_HID_KEYBOARD )||
                                check_cod((bt_bdaddr_t*)p_data->conn.bda, COD_HID_COMBO))
                        BTA_HhSetIdle(p_data->conn.handle, 0);
                    btif_hh_cb.p_curr_dev = btif_hh_find_connected_dev_by_handle(p_data->conn.handle);
                    BTA_HhGetDscpInfo(p_data->conn.handle);
                    p_dev->dev_status = BTHH_CONN_STATE_CONNECTED;
                    HAL_CBACK(bt_hh_callbacks, connection_state_cb,&(p_dev->bd_addr), p_dev->dev_status);
 }
 }
 else {
 bt_bdaddr_t *bdaddr = (bt_bdaddr_t*)p_data->conn.bda;
                btif_dm_hh_open_failed(bdaddr);
                p_dev = btif_hh_find_dev_by_bda(bdaddr);
 if (p_dev != NULL) {
 if(p_dev->vup_timer_active)
                        btif_hh_stop_vup_timer(&(p_dev->bd_addr));
 if (p_dev->fd >= 0) {
                        bta_hh_co_destroy(p_dev->fd);
                        p_dev->fd = -1;
 }
                    p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;
 }
                HAL_CBACK(bt_hh_callbacks, connection_state_cb, (bt_bdaddr_t*) &p_data->conn.bda,BTHH_CONN_STATE_DISCONNECTED);
                btif_hh_cb.status = BTIF_HH_DEV_DISCONNECTED;
 }
 break;

 case BTA_HH_CLOSE_EVT:
            BTIF_TRACE_DEBUG("BTA_HH_CLOSE_EVT: status = %d, handle = %d",
            p_data->dev_status.status, p_data->dev_status.handle);
            p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
 if (p_dev != NULL) {
                BTIF_TRACE_DEBUG("%s: uhid fd = %d", __FUNCTION__, p_dev->fd);
 if(p_dev->vup_timer_active)
                    btif_hh_stop_vup_timer(&(p_dev->bd_addr));
 if (p_dev->fd >= 0) {
                    bta_hh_co_destroy(p_dev->fd);
                    p_dev->fd = -1;
 }
                btif_hh_cb.status = BTIF_HH_DEV_DISCONNECTED;
                p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;
                HAL_CBACK(bt_hh_callbacks, connection_state_cb,&(p_dev->bd_addr), p_dev->dev_status);
 }
 else {
                BTIF_TRACE_WARNING("Error: cannot find device with handle %d", p_data->dev_status.handle);
 }
 break;

 case BTA_HH_GET_RPT_EVT: {
            BT_HDR *hdr = p_data->hs_data.rsp_data.p_rpt_data;
            UINT8 *data = NULL;
            UINT16 len = 0;

            BTIF_TRACE_DEBUG("BTA_HH_GET_RPT_EVT: status = %d, handle = %d",
                 p_data->hs_data.status, p_data->hs_data.handle);
            p_dev = btif_hh_find_connected_dev_by_handle(p_data->hs_data.handle);
 if (p_dev) {
  
 if (hdr) {
                    data = (UINT8 *)(hdr + 1) + hdr->offset;
                    len = hdr->len;
                    HAL_CBACK(bt_hh_callbacks, get_report_cb,
 (bt_bdaddr_t*) &(p_dev->bd_addr),
 (bthh_status_t) p_data->hs_data.status, data, len);
 } else {
                    HAL_CBACK(bt_hh_callbacks, handshake_cb,
 (bt_bdaddr_t*) &(p_dev->bd_addr),
 (bthh_status_t) p_data->hs_data.status);
 }
 } else {
                BTIF_TRACE_WARNING("Error: cannot find device with handle %d", p_data->hs_data.handle);
 }
 break;
 }

 case BTA_HH_SET_RPT_EVT:
            BTIF_TRACE_DEBUG("BTA_HH_SET_RPT_EVT: status = %d, handle = %d",
            p_data->dev_status.status, p_data->dev_status.handle);
            p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
 if (p_dev != NULL) {
                HAL_CBACK(bt_hh_callbacks, handshake_cb,
 (bt_bdaddr_t*) &(p_dev->bd_addr),
 (bthh_status_t) p_data->hs_data.status);
 }
 break;

 case BTA_HH_GET_PROTO_EVT:
            p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
            BTIF_TRACE_WARNING("BTA_HH_GET_PROTO_EVT: status = %d, handle = %d, proto = [%d], %s",
                 p_data->hs_data.status, p_data->hs_data.handle,
                 p_data->hs_data.rsp_data.proto_mode,
 (p_data->hs_data.rsp_data.proto_mode == BTA_HH_PROTO_RPT_MODE) ? "Report Mode" :
 (p_data->hs_data.rsp_data.proto_mode == BTA_HH_PROTO_BOOT_MODE) ? "Boot Mode" : "Unsupported");
 if (p_data->hs_data.rsp_data.proto_mode != BTA_HH_PROTO_UNKNOWN) {
                HAL_CBACK(bt_hh_callbacks, protocol_mode_cb,
 (bt_bdaddr_t*) &(p_dev->bd_addr),
 (bthh_status_t)p_data->hs_data.status,
 (bthh_protocol_mode_t) p_data->hs_data.rsp_data.proto_mode);
 } else {
                HAL_CBACK(bt_hh_callbacks, handshake_cb,
 (bt_bdaddr_t*) &(p_dev->bd_addr),
 (bthh_status_t)p_data->hs_data.status);
 }
 break;

 case BTA_HH_SET_PROTO_EVT:
            BTIF_TRACE_DEBUG("BTA_HH_SET_PROTO_EVT: status = %d, handle = %d",
                 p_data->dev_status.status, p_data->dev_status.handle);
            p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
 if (p_dev) {
                HAL_CBACK(bt_hh_callbacks, handshake_cb,
 (bt_bdaddr_t*)&(p_dev->bd_addr),
 (bthh_status_t)p_data->hs_data.status);
 }
 break;

 case BTA_HH_GET_IDLE_EVT:
            BTIF_TRACE_DEBUG("BTA_HH_GET_IDLE_EVT: handle = %d, status = %d, rate = %d",
                 p_data->hs_data.handle, p_data->hs_data.status,
                 p_data->hs_data.rsp_data.idle_rate);
 break;

 case BTA_HH_SET_IDLE_EVT:
            BTIF_TRACE_DEBUG("BTA_HH_SET_IDLE_EVT: status = %d, handle = %d",
            p_data->dev_status.status, p_data->dev_status.handle);
 break;

 case BTA_HH_GET_DSCP_EVT:
            len = p_data->dscp_info.descriptor.dl_len;
            BTIF_TRACE_DEBUG("BTA_HH_GET_DSCP_EVT: len = %d", len);
            p_dev = btif_hh_cb.p_curr_dev;
 if (p_dev == NULL) {
                BTIF_TRACE_ERROR("BTA_HH_GET_DSCP_EVT: No HID device is currently connected");
 return;
 }
 if (p_dev->fd < 0) {
                LOG_ERROR("BTA_HH_GET_DSCP_EVT: Error, failed to find the uhid driver...");
 return;
 }
 {
 char *cached_name = NULL;
 bt_bdname_t bdname;
 bt_property_t prop_name;
                BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_BDNAME,
 sizeof(bt_bdname_t), &bdname);
 if (btif_storage_get_remote_device_property(
 &p_dev->bd_addr, &prop_name) == BT_STATUS_SUCCESS)
 {
                    cached_name = (char *)bdname.name;
 }
 else
 {
                    cached_name = "Bluetooth HID";
 }

                BTIF_TRACE_WARNING("%s: name = %s", __FUNCTION__, cached_name);
                bta_hh_co_send_hid_info(p_dev, cached_name,
                    p_data->dscp_info.vendor_id, p_data->dscp_info.product_id,
                    p_data->dscp_info.version,   p_data->dscp_info.ctry_code,
                    len, p_data->dscp_info.descriptor.dsc_list);
 if (btif_hh_add_added_dev(p_dev->bd_addr, p_dev->attr_mask)) {
                    BD_ADDR bda;
                    bdcpy(bda, p_dev->bd_addr.address);
                    tBTA_HH_DEV_DSCP_INFO dscp_info;
 bt_status_t ret;
                    bdcpy(bda, p_dev->bd_addr.address);
                    btif_hh_copy_hid_info(&dscp_info, &p_data->dscp_info);
                    BTIF_TRACE_DEBUG("BTA_HH_GET_DSCP_EVT:bda = %02x:%02x:%02x:%02x:%02x:%02x",
                              p_dev->bd_addr.address[0], p_dev->bd_addr.address[1],
                              p_dev->bd_addr.address[2],p_dev->bd_addr.address[3],
                              p_dev->bd_addr.address[4], p_dev->bd_addr.address[5]);
                    BTA_HhAddDev(bda, p_dev->attr_mask,p_dev->sub_class,p_dev->app_id, dscp_info);
                    ret = btif_storage_add_hid_device_info(&(p_dev->bd_addr), p_dev->attr_mask,p_dev->sub_class,p_dev->app_id,
                                                        p_data->dscp_info.vendor_id, p_data->dscp_info.product_id,
                                                        p_data->dscp_info.version,   p_data->dscp_info.ctry_code,
                                                        p_data->dscp_info.ssr_max_latency, p_data->dscp_info.ssr_min_tout,
                                                        len, p_data->dscp_info.descriptor.dsc_list);

                    ASSERTC(ret == BT_STATUS_SUCCESS, "storing hid info failed", ret);
                    BTIF_TRACE_WARNING("BTA_HH_GET_DSCP_EVT: Called add device");

 if (dscp_info.descriptor.dl_len >0 && dscp_info.descriptor.dsc_list != NULL)
 {
                      GKI_freebuf(dscp_info.descriptor.dsc_list);
                      dscp_info.descriptor.dsc_list = NULL;
                      dscp_info.descriptor.dl_len=0;
 }
 }
 else {
                    BTIF_TRACE_WARNING("%s: Device already added ",__FUNCTION__);
 }
  
                tmplen = sizeof(hid_kb_numlock_on_list)
 / sizeof(tHID_KB_LIST);
 for(i = 0; i< tmplen; i++)
 {
 if(p_data->dscp_info.vendor_id
 == hid_kb_numlock_on_list[i].version_id &&
                        p_data->dscp_info.product_id
 == hid_kb_numlock_on_list[i].product_id)
 {
                        BTIF_TRACE_DEBUG("%s() idx[%d] Enabling "\
 "NUMLOCK for device :: %s", __FUNCTION__,
                            i, hid_kb_numlock_on_list[i].kb_name);
  
                        set_keylockstate(BTIF_HH_KEYSTATE_MASK_NUMLOCK,
                                        TRUE);
                        sync_lockstate_on_connect(p_dev);
  
 break;
 }
 }
 }
 break;

 case BTA_HH_ADD_DEV_EVT:
            BTIF_TRACE_WARNING("BTA_HH_ADD_DEV_EVT: status = %d, handle = %d",p_data->dev_info.status, p_data->dev_info.handle);
 int i;
 for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
 if (memcmp(btif_hh_cb.added_devices[i].bd_addr.address, p_data->dev_info.bda, 6) == 0) {
 if (p_data->dev_info.status == BTA_HH_OK) {
                        btif_hh_cb.added_devices[i].dev_handle = p_data->dev_info.handle;
 }
 else {
                        memset(btif_hh_cb.added_devices[i].bd_addr.address, 0, 6);
                        btif_hh_cb.added_devices[i].dev_handle = BTA_HH_INVALID_HANDLE;
 }
 break;
 }
 }
 break;
 case BTA_HH_RMV_DEV_EVT:
                BTIF_TRACE_DEBUG("BTA_HH_RMV_DEV_EVT: status = %d, handle = %d",
                     p_data->dev_info.status, p_data->dev_info.handle);
                BTIF_TRACE_DEBUG("BTA_HH_RMV_DEV_EVT:bda = %02x:%02x:%02x:%02x:%02x:%02x",
                     p_data->dev_info.bda[0], p_data->dev_info.bda[1], p_data->dev_info.bda[2],
                     p_data->dev_info.bda[3], p_data->dev_info.bda[4], p_data->dev_info.bda[5]);
 break;


 case BTA_HH_VC_UNPLUG_EVT:
                BTIF_TRACE_DEBUG("BTA_HH_VC_UNPLUG_EVT: status = %d, handle = %d",
                     p_data->dev_status.status, p_data->dev_status.handle);
                p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
                btif_hh_cb.status = BTIF_HH_DEV_DISCONNECTED;
 if (p_dev != NULL) {
                    BTIF_TRACE_DEBUG("BTA_HH_VC_UNPLUG_EVT:bda = %02x:%02x:%02x:%02x:%02x:%02x",
                         p_dev->bd_addr.address[0], p_dev->bd_addr.address[1],
                         p_dev->bd_addr.address[2],p_dev->bd_addr.address[3],
                         p_dev->bd_addr.address[4], p_dev->bd_addr.address[5]);
  
 if(p_dev->vup_timer_active)
 {
                        btif_hh_stop_vup_timer(&(p_dev->bd_addr));
 }
                    p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;
                    BTIF_TRACE_DEBUG("%s---Sending connection state change", __FUNCTION__);
                    HAL_CBACK(bt_hh_callbacks, connection_state_cb,&(p_dev->bd_addr), p_dev->dev_status);
                    BTIF_TRACE_DEBUG("%s---Removing HID bond", __FUNCTION__);
  
 if (p_dev->local_vup  || check_cod_hid(&(p_dev->bd_addr), COD_HID_MAJOR))
 {
                        p_dev->local_vup = FALSE;
                        BTA_DmRemoveDevice((UINT8 *)p_dev->bd_addr.address);
 }
 else
                        btif_hh_remove_device(p_dev->bd_addr);
                    HAL_CBACK(bt_hh_callbacks, virtual_unplug_cb,&(p_dev->bd_addr),
                                    p_data->dev_status.status);
 }
 break;

 case BTA_HH_API_ERR_EVT  :
                LOG_INFO("BTA_HH API_ERR");
 break;



 default:
                BTIF_TRACE_WARNING("%s: Unhandled event: %d", __FUNCTION__, event);
 break;
 }
}
