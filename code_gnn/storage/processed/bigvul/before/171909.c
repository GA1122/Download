static void btif_hl_proc_reg_request(UINT8 app_idx, UINT8  app_id,
                                     tBTA_HL_REG_PARAM *p_reg_param,
                                     tBTA_HL_CBACK *p_cback){
    UNUSED(p_cback);

    BTIF_TRACE_DEBUG("%s app_idx=%d app_id=%d", __FUNCTION__, app_idx, app_id);

 if(reg_counter >1)
 {
        BTIF_TRACE_DEBUG("btif_hl_proc_reg_request: calling uPDATE");
        BTA_HlUpdate(app_id, p_reg_param,TRUE, btif_hl_cback);
 }
 else
        BTA_HlRegister(app_id, p_reg_param, btif_hl_cback);
}
