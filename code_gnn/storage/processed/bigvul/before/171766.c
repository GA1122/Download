bt_status_t btif_dm_get_remote_services_by_transport(bt_bdaddr_t *remote_addr, const int transport)
{
    BTIF_TRACE_EVENT("%s", __func__);

  
    tBTA_SERVICE_MASK_EXT mask_ext;
    mask_ext.num_uuid = 0;
    mask_ext.p_uuid = NULL;
    mask_ext.srvc_mask = BTA_ALL_SERVICE_MASK;

    BTA_DmDiscoverByTransport(remote_addr->address, &mask_ext,
                   bte_dm_search_services_evt, TRUE, transport);

 return BT_STATUS_SUCCESS;
}
