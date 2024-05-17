void btif_dm_proc_io_rsp(BD_ADDR bd_addr, tBTA_IO_CAP io_cap,
                      tBTA_OOB_DATA oob_data, tBTA_AUTH_REQ auth_req)
{
    UNUSED (bd_addr);
    UNUSED (oob_data);

 if(auth_req & BTA_AUTH_BONDS)
 {
        BTIF_TRACE_DEBUG("%s auth_req:%d", __FUNCTION__, auth_req);
        pairing_cb.auth_req = auth_req;
        pairing_cb.io_cap = io_cap;
 }
}
