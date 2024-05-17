static void btm_sec_auth_collision (UINT16 handle)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if (!btm_cb.collision_start_time)
        btm_cb.collision_start_time = GKI_get_tick_count ();

 if ((GKI_get_tick_count () - btm_cb.collision_start_time) < btm_cb.max_collision_delay)
 {
 if (handle == BTM_SEC_INVALID_HANDLE)
 {
 if ((p_dev_rec = btm_sec_find_dev_by_sec_state (BTM_SEC_STATE_AUTHENTICATING)) == NULL)
                p_dev_rec = btm_sec_find_dev_by_sec_state (BTM_SEC_STATE_ENCRYPTING);
 }
 else
            p_dev_rec = btm_find_dev_by_handle (handle);

 if (p_dev_rec != NULL)
 {
            BTM_TRACE_DEBUG ("btm_sec_auth_collision: state %d (retrying in a moment...)", p_dev_rec->sec_state);
  
 if (p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING || p_dev_rec->sec_state == BTM_SEC_STATE_ENCRYPTING)
                p_dev_rec->sec_state = 0;

            btm_cb.p_collided_dev_rec = p_dev_rec;
            btm_cb.sec_collision_tle.param = (UINT32) btm_sec_collision_timeout;
            btu_start_timer (&btm_cb.sec_collision_tle, BTU_TTYPE_USER_FUNC, BT_1SEC_TIMEOUT);
 }
 }
}
