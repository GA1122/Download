static BOOLEAN btm_sec_start_get_name (tBTM_SEC_DEV_REC *p_dev_rec)
{
    UINT8 tempstate = p_dev_rec->sec_state;

    p_dev_rec->sec_state = BTM_SEC_STATE_GETTING_NAME;

  
  
 if ((btm_initiate_rem_name (p_dev_rec->bd_addr, NULL, BTM_RMT_NAME_SEC,
 0, NULL)) != BTM_CMD_STARTED)
 {
        p_dev_rec->sec_state = tempstate;
 return(FALSE);
 }

 return(TRUE);
}
