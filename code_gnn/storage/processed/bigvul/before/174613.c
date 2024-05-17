tBTM_SEC_DEV_REC *btm_sec_find_dev_by_sec_state (UINT8 state)
{
    tBTM_SEC_DEV_REC *p_dev_rec = &btm_cb.sec_dev_rec[0];
 int i;

 for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++)
 {
 if ((p_dev_rec->sec_flags & BTM_SEC_IN_USE)
 && (p_dev_rec->sec_state == state))
 return(p_dev_rec);
 }
 return(NULL);
}
