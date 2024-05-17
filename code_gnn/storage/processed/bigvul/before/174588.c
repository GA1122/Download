void btm_sec_abort_access_req (BD_ADDR bd_addr)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev (bd_addr);

 if (!p_dev_rec)
 return;

 if (btm_cb.api.p_abort_callback)
 (*btm_cb.api.p_abort_callback)(bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);

 if ((p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING)
 && (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING))
 return;

    p_dev_rec->sec_state  = BTM_SEC_STATE_IDLE;
    p_dev_rec->p_callback = NULL;
}
