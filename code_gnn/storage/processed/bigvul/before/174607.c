void btm_sec_device_down (void)
{
    BTM_TRACE_EVENT ("btm_sec_device_down()  State: %s", btm_pair_state_descr(btm_cb.pairing_state));

    btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
}
