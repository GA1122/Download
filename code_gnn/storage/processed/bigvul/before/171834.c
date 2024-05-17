static void btif_hl_clean_delete_mdl(btif_hl_delete_mdl_t *p_cb)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__ );
    memset(p_cb, 0 , sizeof(btif_hl_delete_mdl_t));
}
