static void btm_sec_connect_after_reject_timeout (TIMER_LIST_ENT *p_tle)
{
    tBTM_SEC_DEV_REC *p_dev_rec = btm_cb.p_collided_dev_rec;
    UNUSED(p_tle);

    BTM_TRACE_EVENT ("btm_sec_connect_after_reject_timeout()");
    btm_cb.sec_collision_tle.param = 0;
    btm_cb.p_collided_dev_rec = 0;

 if (btm_sec_dd_create_conn(p_dev_rec) != BTM_CMD_STARTED)
 {
        BTM_TRACE_WARNING ("Security Manager: btm_sec_connect_after_reject_timeout: failed to start connection");

        btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,  p_dev_rec->dev_class,
                                                p_dev_rec->sec_bd_name, HCI_ERR_MEMORY_FULL);
 }
}
