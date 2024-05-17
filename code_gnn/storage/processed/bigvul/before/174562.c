BOOLEAN  BTM_SecRegister (tBTM_APPL_INFO *p_cb_info)
{
#if BLE_INCLUDED == TRUE
    BT_OCTET16      temp_value = {0};
#endif

    BTM_TRACE_EVENT ("BTM_Sec: application registered");

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
 if (p_cb_info->p_le_callback)
 {
        BTM_TRACE_ERROR ("BTM_SecRegister:p_cb_info->p_le_callback == 0x%x ", p_cb_info->p_le_callback);

 if (p_cb_info->p_le_callback)
 {
 #if SMP_INCLUDED == TRUE
            BTM_TRACE_EVENT ("BTM_Sec: SMP_Register( btm_proc_smp_cback )");
            SMP_Register(btm_proc_smp_cback);
 #endif
  
 if (memcmp(btm_cb.devcb.id_keys.ir, &temp_value, sizeof(BT_OCTET16)) == 0)
 {
                btm_ble_reset_id();
 }
 }
 else
 {
            BTM_TRACE_ERROR ("BTM_SecRegister:p_cb_info->p_le_callback == NULL ");
 }
 }
#endif

    btm_cb.api = *p_cb_info;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
     BTM_TRACE_ERROR ("BTM_SecRegister: btm_cb.api.p_le_callback = 0x%x ", btm_cb.api.p_le_callback);
#endif
    BTM_TRACE_EVENT ("BTM_Sec: application registered");
 return(TRUE);
}
