BOOLEAN bta_hl_co_advrtise_source_sdp(UINT8 app_id)
{
    BOOLEAN     advertize_source_sdp=FALSE;
    UINT8       app_idx;

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
        advertize_source_sdp = p_btif_hl_cb->acb[app_idx].sup_feature.advertize_source_sdp;
 }


    BTIF_TRACE_DEBUG("%s advertize_flag=%d", __FUNCTION__, advertize_source_sdp );

 return advertize_source_sdp;
}
