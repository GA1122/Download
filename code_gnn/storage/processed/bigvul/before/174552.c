UINT32 * BTM_ReadTrustedMask (BD_ADDR bd_addr)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if ((p_dev_rec = btm_find_dev (bd_addr)) != NULL)
 {
 return(p_dev_rec->trusted_mask);
 }
 else
 {
 return NULL;
 }
}
