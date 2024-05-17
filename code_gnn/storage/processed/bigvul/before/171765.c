bt_status_t btif_dm_get_remote_services(bt_bdaddr_t *remote_addr)
{
 bdstr_t bdstr;

    BTIF_TRACE_EVENT("%s: remote_addr=%s", __FUNCTION__, bdaddr_to_string(remote_addr, bdstr, sizeof(bdstr)));

    BTA_DmDiscover(remote_addr->address, BTA_ALL_SERVICE_MASK,
                   bte_dm_search_services_evt, TRUE);

 return BT_STATUS_SUCCESS;
}
