tBTM_STATUS BTM_SecUseMasterLinkKey (BOOLEAN use_master_key)
{
 return(btsnd_hcic_master_link_key (use_master_key) ?  BTM_SUCCESS :
           BTM_NO_RESOURCES);
}
