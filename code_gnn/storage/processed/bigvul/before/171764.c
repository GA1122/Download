bt_status_t btif_dm_get_remote_service_record(bt_bdaddr_t *remote_addr,
 bt_uuid_t *uuid)
{
    tSDP_UUID sdp_uuid;
 bdstr_t bdstr;

    BTIF_TRACE_EVENT("%s: remote_addr=%s", __FUNCTION__, bdaddr_to_string(remote_addr, bdstr, sizeof(bdstr)));

    sdp_uuid.len = MAX_UUID_SIZE;
    memcpy(sdp_uuid.uu.uuid128, uuid->uu, MAX_UUID_SIZE);

    BTA_DmDiscoverUUID(remote_addr->address, &sdp_uuid,
                       bte_dm_remote_service_record_evt, TRUE);

 return BT_STATUS_SUCCESS;
}
