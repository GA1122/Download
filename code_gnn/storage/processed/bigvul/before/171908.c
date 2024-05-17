static void btif_hl_proc_reg_cfm(tBTA_HL *p_data){
 btif_hl_app_cb_t *p_acb;
    UINT8                  app_idx;
 bthl_app_reg_state_t   state = BTHL_APP_REG_STATE_REG_SUCCESS;

    BTIF_TRACE_DEBUG("%s reg status=%d app_handle=%d", __FUNCTION__, p_data->reg_cfm.status, p_data->reg_cfm.app_handle);

 if (btif_hl_find_app_idx(p_data->reg_cfm.app_id, &app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (p_data->reg_cfm.status == BTA_HL_STATUS_OK)
 {
            p_acb->app_handle = p_data->reg_cfm.app_handle;
 }
 else
 {
            btif_hl_free_app_idx(app_idx);
            reg_counter--;
            state = BTHL_APP_REG_STATE_REG_FAILED;
 }

        BTIF_TRACE_DEBUG("%s call reg state callback app_id=%d reg state=%d", __FUNCTION__,  p_data->reg_cfm.app_id, state);
        BTIF_HL_CALL_CBACK(bt_hl_callbacks, app_reg_state_cb, ((int) p_data->reg_cfm.app_id), state );
 }
}
