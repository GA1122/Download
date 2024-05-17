static void btm_sec_collision_timeout (TIMER_LIST_ENT *p_tle)
{
    tBTM_STATUS status;
    UNUSED(p_tle);

    BTM_TRACE_EVENT ("btm_sec_collision_timeout()");
    btm_cb.sec_collision_tle.param = 0;

    status = btm_sec_execute_procedure (btm_cb.p_collided_dev_rec);

  
 if (status != BTM_CMD_STARTED)
 {
  
        btm_sec_dev_rec_cback_event (btm_cb.p_collided_dev_rec, status, FALSE);
 }
}
