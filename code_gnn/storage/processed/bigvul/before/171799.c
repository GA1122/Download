static void search_devices_copy_cb(UINT16 event, char *p_dest, char *p_src)
{
    tBTA_DM_SEARCH *p_dest_data = (tBTA_DM_SEARCH *) p_dest;
    tBTA_DM_SEARCH *p_src_data = (tBTA_DM_SEARCH *) p_src;

 if (!p_src)
 return;

    BTIF_TRACE_DEBUG("%s: event=%s", __FUNCTION__, dump_dm_search_event(event));
    memcpy(p_dest_data, p_src_data, sizeof(tBTA_DM_SEARCH));
 switch (event)
 {
 case BTA_DM_INQ_RES_EVT:
 {
 if (p_src_data->inq_res.p_eir)
 {
                p_dest_data->inq_res.p_eir = (UINT8 *)(p_dest + sizeof(tBTA_DM_SEARCH));
                memcpy(p_dest_data->inq_res.p_eir, p_src_data->inq_res.p_eir, HCI_EXT_INQ_RESPONSE_LEN);
 }
 }
 break;

 case BTA_DM_DISC_RES_EVT:
 {
 if (p_src_data->disc_res.raw_data_size && p_src_data->disc_res.p_raw_data)
 {
                p_dest_data->disc_res.p_raw_data = (UINT8 *)(p_dest + sizeof(tBTA_DM_SEARCH));
                memcpy(p_dest_data->disc_res.p_raw_data,
                    p_src_data->disc_res.p_raw_data, p_src_data->disc_res.raw_data_size);
 }
 }
 break;
 }
}
