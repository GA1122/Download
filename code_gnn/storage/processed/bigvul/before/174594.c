void btm_sec_check_pending_reqs (void)
{
    tBTM_SEC_QUEUE_ENTRY    *p_e;
    BUFFER_Q                bq;

 if (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE)
 {
  
 if (btm_cb.sec_req_pending)
 {
            btm_cb.sec_req_pending = FALSE;
            l2cu_resubmit_pending_sec_req (NULL);
 }

  
        bq = btm_cb.sec_pending_q;

        GKI_init_q (&btm_cb.sec_pending_q);

 while ((p_e = (tBTM_SEC_QUEUE_ENTRY *)GKI_dequeue (&bq)) != NULL)
 {
  
 if (btm_bda_to_acl(p_e->bd_addr, BT_TRANSPORT_BR_EDR) != NULL)
 {
                BTM_TRACE_EVENT ("btm_sec_check_pending_reqs() submitting  PSM: 0x%04x  Is_Orig: %u  mx_proto_id: %u  mx_chan_id: %u",
                                  p_e->psm, p_e->is_orig, p_e->mx_proto_id, p_e->mx_chan_id);

                btm_sec_mx_access_request (p_e->bd_addr, p_e->psm, p_e->is_orig,
                                           p_e->mx_proto_id, p_e->mx_chan_id,
                                           p_e->p_callback, p_e->p_ref_data);
 }

            GKI_freebuf (p_e);
 }
 }
}
