void btm_sec_init (UINT8 sec_mode)
{
#if 0   
 int i;

    memset (btm_cb.sec_serv_rec, 0, sizeof (btm_cb.sec_serv_rec));
    memset (btm_cb.sec_dev_rec, 0, sizeof (btm_cb.sec_dev_rec));
    memset (&btm_cb.pairing_tle, 0, sizeof(TIMER_LIST_ENT));

#endif
    btm_cb.security_mode = sec_mode;
    memset (btm_cb.pairing_bda, 0xff, BD_ADDR_LEN);
    btm_cb.max_collision_delay = BTM_SEC_MAX_COLLISION_DELAY;
}
