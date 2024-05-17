tBTM_STATUS BTM_SecGetDeviceLinkKey (BD_ADDR bd_addr, LINK_KEY link_key)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if (((p_dev_rec = btm_find_dev (bd_addr)) != NULL)
 && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))
 {
        memcpy (link_key, p_dev_rec->link_key, LINK_KEY_LEN);
 return(BTM_SUCCESS);
 }
 return(BTM_UNKNOWN_ADDR);
}
