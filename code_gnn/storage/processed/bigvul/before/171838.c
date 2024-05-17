void btif_hl_clean_mdls_using_app_idx( UINT8 app_idx){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
 btif_hl_mdl_cb_t *p_dcb;
    UINT8 j,x,y;
 bt_bdaddr_t     bd_addr;

        p_acb =BTIF_HL_GET_APP_CB_PTR(app_idx);
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if (p_acb->mcb[j].in_use)
 {
                    p_mcb = &p_acb->mcb[j];
                    BTIF_TRACE_DEBUG("btif_hl_find_mcl_idx_using_mdl_id: mcl handle found j =%d",j);
 for (x=0; x < BTA_HL_NUM_MDLS_PER_MCL ; x ++)
 {
 if (p_mcb->mdl[x].in_use)
 {
                            p_dcb = BTIF_HL_GET_MDL_CB_PTR(app_idx, j,x);
                            btif_hl_release_socket(app_idx,j,x);
 for (y=0; y<6; y++)
 {
                                bd_addr.address[y] = p_mcb->bd_addr[y];
 }
                            BTIF_HL_CALL_CBACK(bt_hl_callbacks, channel_state_cb,  p_acb->app_id,
 &bd_addr, p_dcb->local_mdep_cfg_idx,
                                               p_dcb->channel_id, BTHL_CONN_STATE_DISCONNECTED, 0 );
                            btif_hl_clean_mdl_cb(p_dcb);
 if (!btif_hl_num_dchs_in_use(p_mcb->mcl_handle))
                                    BTA_HlCchClose(p_mcb->mcl_handle);
                            BTIF_TRACE_DEBUG("remote DCH close success mdl_idx=%d", x);
 }
 }
 }
 }
}
