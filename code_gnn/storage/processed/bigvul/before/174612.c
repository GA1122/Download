BOOLEAN btm_sec_find_bonded_dev (UINT8 start_idx, UINT8 *p_found_idx, tBTM_SEC_DEV_REC **p_rec)
{
    BOOLEAN found= FALSE;

#if (SMP_INCLUDED== TRUE)
    tBTM_SEC_DEV_REC *p_dev_rec;
 int i;
 if (start_idx >= BTM_SEC_MAX_DEVICE_RECORDS)
 {
        BTM_TRACE_DEBUG ("LE bonded device not found");
 return found;
 }

    p_dev_rec = &btm_cb.sec_dev_rec[start_idx];
 for (i = start_idx; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++)
 {
 if (p_dev_rec->ble.key_type || (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))
 {
 *p_found_idx = i;
 *p_rec = p_dev_rec;
 break;
 }
 }
    BTM_TRACE_DEBUG ("btm_sec_find_bonded_dev=%d", found);
#endif
 return(found);
}
