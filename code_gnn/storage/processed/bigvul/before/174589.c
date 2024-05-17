BOOLEAN btm_sec_are_all_trusted(UINT32 p_mask[])
{
    UINT32 trusted_inx;
 for (trusted_inx = 0; trusted_inx < BTM_SEC_SERVICE_ARRAY_SIZE; trusted_inx++)
 {
 if (p_mask[trusted_inx] != BTM_SEC_TRUST_ALL)
 return(FALSE);
 }

 return(TRUE);
}
