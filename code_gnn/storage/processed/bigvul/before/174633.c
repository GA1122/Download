static BOOLEAN btm_sec_start_encryption (tBTM_SEC_DEV_REC *p_dev_rec)
{
 if (!btsnd_hcic_set_conn_encrypt (p_dev_rec->hci_handle, TRUE))
 return(FALSE);

    p_dev_rec->sec_state = BTM_SEC_STATE_ENCRYPTING;
 return(TRUE);
}
