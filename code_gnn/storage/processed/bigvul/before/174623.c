void btm_sec_link_key_request (UINT8 *p_bda)
{
    tBTM_SEC_DEV_REC *p_dev_rec = btm_find_or_alloc_dev (p_bda);

    BTM_TRACE_EVENT ("btm_sec_link_key_request()  BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                      p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

 if( (btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_PIN_REQ) &&
 (btm_cb.collision_start_time != 0) &&
 (memcmp (btm_cb.p_collided_dev_rec->bd_addr, p_bda, BD_ADDR_LEN) == 0) )
 {
        BTM_TRACE_EVENT ("btm_sec_link_key_request() rejecting link key req "
 "State: %d START_TIMEOUT : %d",
             btm_cb.pairing_state, btm_cb.collision_start_time);
        btsnd_hcic_link_key_neg_reply (p_bda);
 return;
 }
 if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)
 {
        btsnd_hcic_link_key_req_reply (p_bda, p_dev_rec->link_key);
 return;
 }

  
    l2c_pin_code_request (p_bda);

  
 if ( (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE)
 || (memcmp (p_bda, btm_cb.pairing_bda, BD_ADDR_LEN) != 0) )
 {
 if (btm_cb.api.p_link_key_req_callback)
 {
 if ((*btm_cb.api.p_link_key_req_callback)(p_bda, p_dev_rec->link_key) == BTM_SUCCESS)
 {
                btsnd_hcic_link_key_req_reply (p_bda, p_dev_rec->link_key);
 return;
 }
 }
 }

  
    btsnd_hcic_link_key_neg_reply (p_bda);
}
