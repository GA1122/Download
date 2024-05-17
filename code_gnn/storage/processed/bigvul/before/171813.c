void btif_hh_start_vup_timer(bt_bdaddr_t *bd_addr)
{
 btif_hh_device_t *p_dev  = btif_hh_find_connected_dev_by_bda(bd_addr);

 if (p_dev->vup_timer_active == FALSE)
 {
        BTIF_TRACE_DEBUG("Start VUP timer ");
        memset(&p_dev->vup_timer, 0, sizeof(TIMER_LIST_ENT));
        p_dev->vup_timer.param = (UINT32)btif_hh_tmr_hdlr;
        btu_start_timer(&p_dev->vup_timer, BTU_TTYPE_USER_FUNC,
                        BTIF_TIMEOUT_VUP_SECS);
 }
 else
 {
        BTIF_TRACE_DEBUG("Restart VUP timer ");
        btu_stop_timer(&p_dev->vup_timer);
        btu_start_timer(&p_dev->vup_timer, BTU_TTYPE_USER_FUNC,
                        BTIF_TIMEOUT_VUP_SECS);
 }
        p_dev->vup_timer_active = TRUE;

}
