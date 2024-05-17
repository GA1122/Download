void btif_hh_stop_vup_timer(bt_bdaddr_t *bd_addr)
{
 btif_hh_device_t *p_dev  = btif_hh_find_connected_dev_by_bda(bd_addr);
 if(p_dev != NULL)
 {
 if (p_dev->vup_timer_active)
 {
            BTIF_TRACE_DEBUG("stop VUP timer ");
            btu_stop_timer(&p_dev->vup_timer);
 }
        p_dev->vup_timer_active = FALSE;
 }
}
