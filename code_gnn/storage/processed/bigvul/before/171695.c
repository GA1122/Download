BOOLEAN bta_hl_co_load_mdl_config (UINT8 app_id, UINT8 buffer_size,
                                   tBTA_HL_MDL_CFG *p_mdl_buf )
{
    BOOLEAN result = TRUE;
    UINT8 i;
    tBTA_HL_MDL_CFG *p;

    BTIF_TRACE_DEBUG("%s app_id=%d, num_items=%d",
                      __FUNCTION__, app_id, buffer_size);

 if (buffer_size > BTA_HL_NUM_MDL_CFGS)
 {
        result = FALSE;
 return result;
 }
    result = btif_hl_load_mdl_config(app_id, buffer_size, p_mdl_buf);

 if (result)
 {
 for (i=0, p=p_mdl_buf; i<buffer_size; i++, p++ )
 {
 if (p->active)
 {
                BTIF_TRACE_DEBUG("i=%d mdl_id=0x%x dch_mode=%d local mdep_role=%d mdep_id=%d mtu=%d",
                                  i, p->mdl_id, p->dch_mode, p->local_mdep_role, p->local_mdep_role, p->mtu);
 }
 }
 }

    BTIF_TRACE_DEBUG("%s success=%d num_items=%d", __FUNCTION__, result, buffer_size);

 return result;
}
