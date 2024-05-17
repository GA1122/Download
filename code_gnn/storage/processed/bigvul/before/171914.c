static void btif_hl_reset_mdep_filter(UINT8 app_idx)
{
 btif_hl_app_cb_t *p_acb  =BTIF_HL_GET_APP_CB_PTR(app_idx);
    p_acb->filter.num_elems = 0;
}
