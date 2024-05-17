BOOLEAN btm_sec_is_le_capable_dev (BD_ADDR bda)
{
    tBTM_SEC_DEV_REC *p_dev_rec= btm_find_dev (bda);
    BOOLEAN le_capable = FALSE;

#if (BLE_INCLUDED== TRUE)
 if (p_dev_rec && ((p_dev_rec->device_type == BT_DEVICE_TYPE_DUMO) ||
 (p_dev_rec->device_type == BT_DEVICE_TYPE_BLE) ) )
 {
        le_capable  = TRUE;
 }
#endif
 return le_capable;
}
