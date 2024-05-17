static bt_status_t btpan_connect(const bt_bdaddr_t *bd_addr, int local_role, int remote_role)
{
    BTIF_TRACE_DEBUG("local_role:%d, remote_role:%d", local_role, remote_role);
 int bta_local_role = btpan_role_to_bta(local_role);
 int bta_remote_role = btpan_role_to_bta(remote_role);
    btpan_new_conn(-1, bd_addr->address, bta_local_role, bta_remote_role);
    BTA_PanOpen((UINT8*)bd_addr->address, bta_local_role, bta_remote_role);
 return BT_STATUS_SUCCESS;
}
