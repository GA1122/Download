BOOLEAN BTM_snd_conn_encrypt (UINT16  handle, BOOLEAN enable)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev_by_handle (handle);

    BTM_TRACE_EVENT ("BTM_snd_conn_encrypt Security Manager: encrypt_change p_dev_rec : 0x%x, enable = %s", p_dev_rec, (enable == TRUE) ? "TRUE" : "FALSE");

 if (!p_dev_rec)
 {
        BTM_TRACE_EVENT ("BTM_snd_conn_encrypt Error no  p_dev_rec : 0x%x\n", p_dev_rec);
 return(FALSE);
 }

 if ( p_dev_rec->sec_state == BTM_SEC_STATE_IDLE)
 {
 if (!btsnd_hcic_set_conn_encrypt (handle, enable))
 return(FALSE);

        p_dev_rec->sec_state = BTM_SEC_STATE_ENCRYPTING;

 return(TRUE);
 }
 else
 return(FALSE);
}
