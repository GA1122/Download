const btrc_interface_t *btif_rc_get_interface(void)
{
    BTIF_TRACE_EVENT("%s", __FUNCTION__);
 return &bt_rc_interface;
}
