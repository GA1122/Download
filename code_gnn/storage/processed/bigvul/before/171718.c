static void btif_in_storage_request_copy_cb(UINT16 event,
 char *p_new_buf, char *p_old_buf)
{
 btif_storage_req_t *new_req = (btif_storage_req_t*)p_new_buf;
 btif_storage_req_t *old_req = (btif_storage_req_t*)p_old_buf;

     BTIF_TRACE_EVENT("%s", __FUNCTION__);
 switch (event)
 {
 case BTIF_CORE_STORAGE_REMOTE_WRITE:
 case BTIF_CORE_STORAGE_ADAPTER_WRITE:
 {
             bdcpy(new_req->write_req.bd_addr.address, old_req->write_req.bd_addr.address);
  
             new_req->write_req.prop.type = old_req->write_req.prop.type;
             new_req->write_req.prop.len = old_req->write_req.prop.len;

             new_req->write_req.prop.val = (UINT8 *)(p_new_buf + sizeof(btif_storage_req_t));
             memcpy(new_req->write_req.prop.val, old_req->write_req.prop.val,
                    old_req->write_req.prop.len);
 }break;
 }
}
