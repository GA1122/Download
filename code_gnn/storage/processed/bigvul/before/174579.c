static BOOLEAN btm_dev_encrypted (tBTM_SEC_DEV_REC *p_dev_rec)
{
 if(p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED)
 {
 return(TRUE);
 }
 return(FALSE);
}
