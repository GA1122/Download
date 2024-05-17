static void btif_hl_proc_dereg_cfm(tBTA_HL *p_data)

{
 btif_hl_app_cb_t *p_acb;
    UINT8                   app_idx;
 int                     app_id = 0;
 bthl_app_reg_state_t    state = BTHL_APP_REG_STATE_DEREG_SUCCESS;

    BTIF_TRACE_DEBUG("%s de-reg status=%d app_handle=%d", __FUNCTION__,
                p_data->dereg_cfm.status, p_data->dereg_cfm.app_handle);

 if (btif_hl_find_app_idx_using_app_id(p_data->dereg_cfm.app_id, &app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
        app_id = (int) p_acb->app_id;
 if (p_data->dereg_cfm.status == BTA_HL_STATUS_OK)
 {
            btif_hl_clean_mdls_using_app_idx(app_idx);
            memset(p_acb, 0,sizeof(btif_hl_app_cb_t));
 }
 else
            state = BTHL_APP_REG_STATE_DEREG_FAILED;

        BTIF_TRACE_DEBUG("call reg state callback app_id=%d state=%d", app_id, state);
        BTIF_HL_CALL_CBACK(bt_hl_callbacks, app_reg_state_cb, app_id, state );

 if (btif_hl_is_no_active_app())
 {
            btif_hl_disable();
 }
 }
}
