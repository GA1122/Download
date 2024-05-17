static void search_services_copy_cb(UINT16 event, char *p_dest, char *p_src)
{
    tBTA_DM_SEARCH *p_dest_data = (tBTA_DM_SEARCH *) p_dest;
    tBTA_DM_SEARCH *p_src_data = (tBTA_DM_SEARCH *) p_src;

 if (!p_src)
 return;
    memcpy(p_dest_data, p_src_data, sizeof(tBTA_DM_SEARCH));
 switch (event)
 {
 case BTA_DM_DISC_RES_EVT:
 {
 if (p_src_data->disc_res.result == BTA_SUCCESS)
 {
 if (p_src_data->disc_res.num_uuids > 0)
 {
                       p_dest_data->disc_res.p_uuid_list =
 (UINT8*)(p_dest + sizeof(tBTA_DM_SEARCH));
                       memcpy(p_dest_data->disc_res.p_uuid_list, p_src_data->disc_res.p_uuid_list,
                              p_src_data->disc_res.num_uuids*MAX_UUID_SIZE);
                       GKI_freebuf(p_src_data->disc_res.p_uuid_list);
 }
 if (p_src_data->disc_res.p_raw_data != NULL)
 {
                      GKI_freebuf(p_src_data->disc_res.p_raw_data);
 }
 }
 } break;
 }
}
