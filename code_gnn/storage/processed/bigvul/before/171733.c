static void execute_storage_remote_request(UINT16 event, char *p_param)
{
 bt_status_t status = BT_STATUS_FAIL;
 bt_property_t prop;

    BTIF_TRACE_EVENT("execute storage remote request event : %d", event);

 switch (event)
 {
 case BTIF_CORE_STORAGE_REMOTE_READ:
 {
 char buf[1024];
 btif_storage_req_t *p_req = (btif_storage_req_t*)p_param;
            prop.type = p_req->read_req.type;
            prop.val = (void*) buf;
            prop.len = sizeof(buf);

            status = btif_storage_get_remote_device_property(&(p_req->read_req.bd_addr),
 &prop);
            HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                            status, &(p_req->read_req.bd_addr), 1, &prop);
 }break;
 case BTIF_CORE_STORAGE_REMOTE_WRITE:
 {
 btif_storage_req_t *p_req = (btif_storage_req_t*)p_param;
           status = btif_storage_set_remote_device_property(&(p_req->write_req.bd_addr),
 &(p_req->write_req.prop));
 }break;
 case BTIF_CORE_STORAGE_REMOTE_READ_ALL:
 {
 btif_storage_req_t *p_req = (btif_storage_req_t*)p_param;
           btif_in_get_remote_device_properties(&p_req->read_req.bd_addr);
 }break;
 }
}
