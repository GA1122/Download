static BOOLEAN btm_dev_authenticated (tBTM_SEC_DEV_REC *p_dev_rec)
{
 if(p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)
 {
 return(TRUE);
 }
 return(FALSE);
}
