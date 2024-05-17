static bt_status_t register_application(bthl_reg_param_t *p_reg_param, int *app_id){
 btif_hl_app_cb_t *p_acb;
    tBTA_HL_SUP_FEATURE         *p_sup;
    tBTA_HL_MDEP_CFG            *p_cfg;
    tBTA_HL_MDEP_DATA_TYPE_CFG  *p_data;
    UINT8                       app_idx=0, i=0;
 bthl_mdep_cfg_t *p_mdep_cfg;
 bt_status_t                 status = BT_STATUS_SUCCESS;
 btif_hl_evt_cb_t            evt_param;
 int                         len;

    CHECK_BTHL_INIT();
    BTIF_TRACE_EVENT("%s", __FUNCTION__);
    btif_hl_display_calling_process_name();

 if (btif_hl_get_state() == BTIF_HL_STATE_DISABLED)
 {
        btif_hl_init();
        btif_hl_set_state(BTIF_HL_STATE_ENABLING);
        BTA_HlEnable(btif_hl_ctrl_cback);
 }

 if (!btif_hl_find_avail_app_idx(&app_idx))
 {
        BTIF_TRACE_ERROR("Unable to allocate a new application control block");
 return BT_STATUS_FAIL;
 }

    p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
    p_acb->in_use = TRUE;


    p_acb->app_id = btif_hl_get_next_app_id();

 if (p_reg_param->application_name != NULL )
        strncpy(p_acb->application_name, p_reg_param->application_name, BTIF_HL_APPLICATION_NAME_LEN);

 if (p_reg_param->provider_name != NULL )
        strncpy(p_acb->provider_name, p_reg_param->provider_name, BTA_PROVIDER_NAME_LEN);

 if (p_reg_param->srv_name != NULL )
        strncpy(p_acb->srv_name, p_reg_param->srv_name, BTA_SERVICE_NAME_LEN);

 if (p_reg_param->srv_desp != NULL )
        strncpy(p_acb->srv_desp, p_reg_param->srv_desp, BTA_SERVICE_DESP_LEN);

    p_sup = &p_acb->sup_feature;
    p_sup->advertize_source_sdp = TRUE;
    p_sup->echo_cfg.max_rx_apdu_size = BTIF_HL_ECHO_MAX_TX_RX_APDU_SIZE;
    p_sup->echo_cfg.max_tx_apdu_size = BTIF_HL_ECHO_MAX_TX_RX_APDU_SIZE;
    p_sup->num_of_mdeps = p_reg_param->number_of_mdeps;

 for (i=0, p_mdep_cfg = p_reg_param->mdep_cfg ; i<  p_sup->num_of_mdeps; i++, p_mdep_cfg++ )
 {
        p_cfg = &p_sup->mdep[i].mdep_cfg;
        p_cfg->num_of_mdep_data_types = 1;
        p_data  = &p_cfg->data_cfg[0];

 if ( !btif_hl_get_bta_mdep_role(p_mdep_cfg->mdep_role, &(p_cfg->mdep_role)))
 {
            BTIF_TRACE_ERROR("Invalid mdep_role=%d", p_mdep_cfg->mdep_role);
            status = BT_STATUS_FAIL;
 break;
 }
 else
 {
 if (p_cfg->mdep_role == BTA_HL_MDEP_ROLE_SINK )
                p_sup->app_role_mask |= BTA_HL_MDEP_ROLE_MASK_SINK;
 else
                p_sup->app_role_mask |=  BTA_HL_MDEP_ROLE_MASK_SOURCE;

 if ( (p_sup->app_role_mask & BTA_HL_MDEP_ROLE_MASK_SINK) &&
 (p_sup->app_role_mask & BTA_HL_MDEP_ROLE_MASK_SINK) )
 {
                p_acb->dev_type = BTA_HL_DEVICE_TYPE_DUAL;
 }
 else if ( p_sup->app_role_mask & BTA_HL_MDEP_ROLE_MASK_SINK )
                p_acb->dev_type = BTA_HL_DEVICE_TYPE_SINK;
 else

                p_acb->dev_type = BTA_HL_DEVICE_TYPE_SOURCE;

            p_data->data_type = (UINT16) p_mdep_cfg->data_type;
            p_data->max_rx_apdu_size = btif_hl_get_max_rx_apdu_size(p_cfg->mdep_role, p_data->data_type);
            p_data->max_tx_apdu_size = btif_hl_get_max_tx_apdu_size(p_cfg->mdep_role, p_data->data_type);

 if (p_mdep_cfg->mdep_description != NULL )
                strncpy(p_data->desp, p_mdep_cfg->mdep_description, BTA_SERVICE_DESP_LEN);

 if ( !btif_hl_get_bta_channel_type(p_mdep_cfg->channel_type, &(p_acb->channel_type[i])))
 {
                BTIF_TRACE_ERROR("Invalid channel_type=%d", p_mdep_cfg->channel_type);
                status = BT_STATUS_FAIL;
 break;
 }
 }
 }

 if (status == BT_STATUS_SUCCESS)
 {
 *app_id = (int) p_acb->app_id;
        evt_param.reg.app_idx = app_idx;
        len = sizeof(btif_hl_reg_t);
        p_acb->reg_pending = TRUE;
        reg_counter++;
        BTIF_TRACE_DEBUG("calling btif_transfer_context status=%d app_id=%d", status, *app_id);
        status = btif_transfer_context (btif_hl_proc_cb_evt, BTIF_HL_REG_APP,
 (char*) &evt_param, len, NULL);
        ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);

 }
 else
 {
        btif_hl_free_app_idx(app_idx);
 }

    BTIF_TRACE_DEBUG("register_application status=%d app_id=%d", status, *app_id);
 return status;
}
